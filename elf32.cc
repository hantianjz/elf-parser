#include "elf_parser.h"
using namespace elf_parser;

Elf32_Ehdr *Elf32::get_header() { return (Elf32_Ehdr *)m_mmap_program; }

void Elf32::print_header() {
  auto *header = get_header();
  printf("%s %d %d\n", get_e_type_str(header->e_type).c_str(),
         header->e_machine, header->e_version);
}

#if 0
std::vector<section_t> Elf32::get_sections() {
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)m_mmap_program;
  Elf64_Shdr *shdr = (Elf64_Shdr *)(m_mmap_program + ehdr->e_shoff);
  int shnum = ehdr->e_shnum;

  Elf64_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
  const char *const sh_strtab_p = (char *)m_mmap_program + sh_strtab->sh_offset;

  std::vector<section_t> sections;
  for (int i = 0; i < shnum; ++i) {
    section_t section;
    section.section_index = i;
    section.section_name = std::string(sh_strtab_p + shdr[i].sh_name);
    section.section_type = get_section_type(shdr[i].sh_type);
    section.section_addr = shdr[i].sh_addr;
    section.section_offset = shdr[i].sh_offset;
    section.section_size = shdr[i].sh_size;
    section.section_ent_size = shdr[i].sh_entsize;
    section.section_addr_align = shdr[i].sh_addralign;

    sections.push_back(section);
  }
  return sections;
}

std::vector<segment_t> Elf32::get_segments() {
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)m_mmap_program;
  Elf64_Phdr *phdr = (Elf64_Phdr *)(m_mmap_program + ehdr->e_phoff);
  int phnum = ehdr->e_phnum;

  Elf64_Shdr *shdr = (Elf64_Shdr *)(m_mmap_program + ehdr->e_shoff);
  Elf64_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
  const char *const sh_strtab_p = (char *)m_mmap_program + sh_strtab->sh_offset;
  (void)sh_strtab_p;

  std::vector<segment_t> segments;
  for (int i = 0; i < phnum; ++i) {
    segment_t segment;
    segment.segment_type = get_segment_type(phdr[i].p_type);
    segment.segment_offset = phdr[i].p_offset;
    segment.segment_virtaddr = phdr[i].p_vaddr;
    segment.segment_physaddr = phdr[i].p_paddr;
    segment.segment_filesize = phdr[i].p_filesz;
    segment.segment_memsize = phdr[i].p_memsz;
    segment.segment_flags = get_segment_flags(phdr[i].p_flags);
    segment.segment_align = phdr[i].p_align;

    segments.push_back(segment);
  }
  return segments;
}

std::vector<symbol_t> Elf32::get_symbols() {
  std::vector<section_t> secs = get_sections();

  // get headers for offsets
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)m_mmap_program;
  Elf64_Shdr *shdr = (Elf64_Shdr *)(m_mmap_program + ehdr->e_shoff);
  (void)shdr;

  // get strtab
  char *sh_strtab_p = nullptr;
  for (auto &sec : secs) {
    if ((sec.section_type == "SHT_STRTAB") && (sec.section_name == ".strtab")) {
      sh_strtab_p = (char *)m_mmap_program + sec.section_offset;
      break;
    }
  }

  // get dynstr
  char *sh_dynstr_p = nullptr;
  for (auto &sec : secs) {
    if ((sec.section_type == "SHT_STRTAB") && (sec.section_name == ".dynstr")) {
      sh_dynstr_p = (char *)m_mmap_program + sec.section_offset;
      break;
    }
  }

  std::vector<symbol_t> symbols;
  for (auto &sec : secs) {
    if ((sec.section_type != "SHT_SYMTAB") &&
        (sec.section_type != "SHT_DYNSYM"))
      continue;

    auto total_syms = sec.section_size / sizeof(Elf64_Sym);
    auto syms_data = (Elf64_Sym *)(m_mmap_program + sec.section_offset);

    for (size_t i = 0; i < total_syms; ++i) {
      symbol_t symbol;
      symbol.symbol_num = i;
      symbol.symbol_value = syms_data[i].st_value;
      symbol.symbol_size = syms_data[i].st_size;
      symbol.symbol_type = get_symbol_type(syms_data[i].st_info);
      symbol.symbol_bind = get_symbol_bind(syms_data[i].st_info);
      symbol.symbol_visibility = get_symbol_visibility(syms_data[i].st_other);
      symbol.symbol_index = get_symbol_index(syms_data[i].st_shndx);
      symbol.symbol_section = sec.section_name;

      if (sec.section_type == "SHT_SYMTAB")
        symbol.symbol_name = std::string(sh_strtab_p + syms_data[i].st_name);

      if (sec.section_type == "SHT_DYNSYM")
        symbol.symbol_name = std::string(sh_dynstr_p + syms_data[i].st_name);

      symbols.push_back(symbol);
    }
  }
  return symbols;
}

std::vector<relocation_t> Elf32::get_relocations() {
  auto secs = get_sections();
  auto syms = get_symbols();

  int plt_entry_size = 0;
  uint64_t plt_vma_address = 0;

  for (auto &sec : secs) {
    if (sec.section_name == ".plt") {
      plt_entry_size = sec.section_ent_size;
      plt_vma_address = sec.section_addr;
      break;
    }
  }

  std::vector<relocation_t> relocations;
  for (auto &sec : secs) {

    if (sec.section_type != "SHT_RELA")
      continue;

    auto total_relas = sec.section_size / sizeof(Elf64_Rela);
    auto relas_data = (Elf64_Rela *)(m_mmap_program + sec.section_offset);

    for (int i = 0; i < total_relas; ++i) {
      relocation_t rel;
      rel.relocation_offset =
          static_cast<std::uintptr_t>(relas_data[i].r_offset);
      rel.relocation_info = static_cast<std::intptr_t>(relas_data[i].r_info);
      rel.relocation_type = get_relocation_type(relas_data[i].r_info);

      rel.relocation_symbol_value =
          get_rel_symbol_value(relas_data[i].r_info, syms);

      rel.relocation_symbol_name =
          get_rel_symbol_name(relas_data[i].r_info, syms);

      rel.relocation_plt_address = plt_vma_address + (i + 1) * plt_entry_size;
      rel.relocation_section_name = sec.section_name;

      relocations.push_back(rel);
    }
  }
  return relocations;
}
#endif
