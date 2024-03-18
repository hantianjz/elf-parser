#include "elf_parser.h"
#include <inttypes.h> // PRIx64
#include <iostream>

#if 0
void print_relocations(std::vector<elf_parser::relocation_t> &relocations) {
  printf("  [*] %s %s %s %s %s\n", "Offset", "Info", "Type", "Sym. Value",
         "Sym. Name (section)");
  printf("  [*] %s\n", "Calculated PLT Address");
  for (auto &rel : relocations) {
    printf("%ld %ld %s %ld (%s) %s\n", rel.relocation_offset,
           rel.relocation_info, rel.relocation_type.c_str(),
           rel.relocation_symbol_value, rel.relocation_symbol_name.c_str(),
           rel.relocation_section_name.c_str());
    printf("%ld\n", rel.relocation_plt_address);
  }
}

void print_sections(std::vector<elf_parser::section_t> &sections) {
  printf("  [Nr] %-16s %-16s %-16s %s\n", "Name", "Type", "Address", "Offset");
  printf("       %-16s %-16s %5s\n", "Size", "EntSize", "Align");

  for (auto &section : sections) {
    printf("  [%2d] %-16s %-16s %016" PRIx64 " %08" PRIx64 "\n",
           section.section_index, section.section_name.c_str(),
           section.section_type.c_str(), section.section_addr,
           section.section_offset);

    printf("       %016zx %016" PRIx64 " %5" PRIu64 "\n", section.section_size,
           section.section_ent_size, section.section_addr_align);
  }
}

void print_segments(std::vector<elf_parser::segment_t> &segments) {
  printf("  %-16s  %-16s   %-16s   %s\n", "Type", "Offset", "VirtAddr",
         "PhysAddr");
  printf("  %-16s  %-16s   %-16s  %6s %5s\n", " ", "FileSiz", "MemSiz", "Flags",
         "Align");

  for (auto &segment : segments) {
    printf("   %-16s 0x%016" PRIx64 " 0x%016" PRIx64 " 0x%016" PRIx64 "\n",
           segment.segment_type.c_str(), segment.segment_offset,
           segment.segment_virtaddr, segment.segment_physaddr);

    printf("   %-16s 0x%016" PRIx64 " 0x%016" PRIx64 " %-5s %-5" PRIx64 "\n",
           "", segment.segment_filesize, segment.segment_memsize,
           segment.segment_flags.c_str(), segment.segment_align);
  }
}

void print_symbols(std::vector<elf_parser::symbol_t> &symbols) {
  printf("Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
  for (auto &symbol : symbols) {
    printf("%-3d: %08" PRIx64 "  %-4d %-8s %-7s %-9s %-3s %s(%s)\n",
           symbol.symbol_num, symbol.symbol_value, symbol.symbol_size,
           symbol.symbol_type.c_str(), symbol.symbol_bind.c_str(),
           symbol.symbol_visibility.c_str(), symbol.symbol_index.c_str(),
           symbol.symbol_name.c_str(), symbol.symbol_section.c_str());
  }
}
#endif

int main(int argc, char *argv[]) {
  char usage_banner[] = "usage: ./elf_walker [<executable>]\n";
  if (argc < 2) {
    std::cerr << usage_banner;
    return -1;
  }

  std::string program((std::string)argv[1]);
  elf_parser::Elf_parser elf_parser(program);
  elf_parser.print_header();

#if 0
  std::vector<elf_parser::relocation_t> relocs = elf_parser.get_relocations();
  print_relocations(relocs);

  std::vector<elf_parser::section_t> secs = elf_parser.get_sections();
  print_sections(secs);

  std::vector<elf_parser::segment_t> segs = elf_parser.get_segments();
  print_segments(segs);

  std::vector<elf_parser::symbol_t> syms = elf_parser.get_symbols();
  print_symbols(syms);
#endif
  return 0;
}
