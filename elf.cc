#include "elf_parser.h"

using namespace elf_parser;

Elf::Elf(uint8_t *m_program) : m_mmap_program{m_program} {
  (void)m_mmap_program;
}

std::string get_e_type_str(unsigned et) {
  if (et >= ET_NUM) {
    fprintf(stderr, "Unknown elf type: %d", et);
    return "UNKNOWN ELF TYPE";
  }

  static const std::string elf_type_str[] = {
      [ET_NONE] = "None",       [ET_REL] = "Relocatable File",
      [ET_EXEC] = "Executable", [ET_DYN] = "Share Object File",
      [ET_CORE] = "Core File",
  };
  return elf_type_str[et];
}

std::string get_section_type(unsigned tt) {
  if (tt >= SHT_NUM) {
    fprintf(stderr, "Unknown section type: %d", tt);
    return "UNKNOWN SECTION TYPE";
  }

  static const std::string section_type_str[] = {
      [SHT_NULL] = "SHT_NULL",
      [SHT_PROGBITS] = "SHT_PROGBITS",
      [SHT_SYMTAB] = "SHT_SYMTAB",
      [SHT_STRTAB] = "SHT_STRTAB",
      [SHT_RELA] = "SHT_RELA",
      [SHT_HASH] = "SHT_HASH",
      [SHT_DYNAMIC] = "SHT_DYNAMIC",
      [SHT_NOTE] = "SHT_NOTE",
      [SHT_NOBITS] = "SHT_NOBITS",
      [SHT_REL] = "SHT_REL",
      [SHT_SHLIB] = "SHT_SHLIB",
      [SHT_DYNSYM] = "SHT_DYNSYM",
      [SHT_INIT_ARRAY] = "SHT_INIT_ARRAY",
      [SHT_FINI_ARRAY] = "SHT_FINI_ARRAY",
      [SHT_PREINIT_ARRAY] = "SHT_PREINIT_ARRAY",
      [SHT_GROUP] = "SHT_GROUP",
      [SHT_SYMTAB_SHNDX] = "SHT_SYMTAB_SHNDX"};
  return section_type_str[tt];
}

std::string get_segment_type(uint32_t &seg_type) {
  switch (seg_type) {
  case PT_NULL:
    return "NULL"; /* Program header table entry unused */
  case PT_LOAD:
    return "LOAD"; /* Loadable program segment */
  case PT_DYNAMIC:
    return "DYNAMIC"; /* Dynamic linking information */
  case PT_INTERP:
    return "INTERP"; /* Program interpreter */
  case PT_NOTE:
    return "NOTE"; /* Auxiliary information */
  case PT_SHLIB:
    return "SHLIB"; /* Reserved */
  case PT_PHDR:
    return "PHDR"; /* Entry for header table itself */
  case PT_TLS:
    return "TLS"; /* Thread-local storage segment */
  case PT_NUM:
    return "NUM"; /* Number of defined types */
  case PT_LOOS:
    return "LOOS"; /* Start of OS-specific */
  case PT_GNU_EH_FRAME:
    return "GNU_EH_FRAME"; /* GCC .eh_frame_hdr segment */
  case PT_GNU_STACK:
    return "GNU_STACK"; /* Indicates stack executability */
  case PT_GNU_RELRO:
    return "GNU_RELRO"; /* Read-only after relocation */
  // case PT_LOSUNW: return "LOSUNW";
  case PT_SUNWBSS:
    return "SUNWBSS"; /* Sun Specific segment */
  case PT_SUNWSTACK:
    return "SUNWSTACK"; /* Stack segment */
  // case PT_HISUNW: return "HISUNW";
  case PT_HIOS:
    return "HIOS"; /* End of OS-specific */
  case PT_LOPROC:
    return "LOPROC"; /* Start of processor-specific */
  case PT_HIPROC:
    return "HIPROC"; /* End of processor-specific */
  default:
    return "UNKNOWN";
  }
}

std::string get_segment_flags(uint32_t &seg_flags) {
  std::string flags;

  if (seg_flags & PF_R)
    flags.append("R");

  if (seg_flags & PF_W)
    flags.append("W");

  if (seg_flags & PF_X)
    flags.append("E");

  return flags;
}

std::string get_symbol_type(uint8_t &sym_type) {
  switch (ELF32_ST_TYPE(sym_type)) {
  case 0:
    return "NOTYPE";
  case 1:
    return "OBJECT";
  case 2:
    return "FUNC";
  case 3:
    return "SECTION";
  case 4:
    return "FILE";
  case 6:
    return "TLS";
  case 7:
    return "NUM";
  case 10:
    return "LOOS";
  case 12:
    return "HIOS";
  default:
    return "UNKNOWN";
  }
}

std::string get_symbol_bind(uint8_t &sym_bind) {
  switch (ELF32_ST_BIND(sym_bind)) {
  case 0:
    return "LOCAL";
  case 1:
    return "GLOBAL";
  case 2:
    return "WEAK";
  case 3:
    return "NUM";
  case 10:
    return "UNIQUE";
  case 12:
    return "HIOS";
  case 13:
    return "LOPROC";
  default:
    return "UNKNOWN";
  }
}

std::string get_symbol_visibility(uint8_t &sym_vis) {
  switch (ELF32_ST_VISIBILITY(sym_vis)) {
  case 0:
    return "DEFAULT";
  case 1:
    return "INTERNAL";
  case 2:
    return "HIDDEN";
  case 3:
    return "PROTECTED";
  default:
    return "UNKNOWN";
  }
}

std::string get_symbol_index(uint16_t &sym_idx) {
  switch (sym_idx) {
  case SHN_ABS:
    return "ABS";
  case SHN_COMMON:
    return "COM";
  case SHN_UNDEF:
    return "UND";
  case SHN_XINDEX:
    return "COM";
  default:
    return std::to_string(sym_idx);
  }
}

std::string get_relocation_type(uint64_t &rela_type) {
  switch (ELF64_R_TYPE(rela_type)) {
  case 1:
    return "R_X86_64_32";
  case 2:
    return "R_X86_64_PC32";
  case 5:
    return "R_X86_64_COPY";
  case 6:
    return "R_X86_64_GLOB_DAT";
  case 7:
    return "R_X86_64_JUMP_SLOT";
  default:
    return "OTHERS";
  }
}

std::intptr_t get_rel_symbol_value(uint64_t &sym_idx,
                                   std::vector<symbol_t> &syms) {
  std::intptr_t sym_val = 0;
  for (auto &sym : syms) {
    if (sym.symbol_num == ELF64_R_SYM(sym_idx)) {
      sym_val = sym.symbol_value;
      break;
    }
  }
  return sym_val;
}

std::string get_rel_symbol_name(uint64_t &sym_idx,
                                std::vector<symbol_t> &syms) {
  std::string sym_name;
  for (auto &sym : syms) {
    if (sym.symbol_num == ELF64_R_SYM(sym_idx)) {
      sym_name = sym.symbol_name;
      break;
    }
  }
  return sym_name;
}
