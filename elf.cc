#include "elf_parser.h"

using namespace elf_parser;

std::string elf_parser::get_e_type_str(unsigned et) {
  if (et >= ET_NUM) {
    fprintf(stderr, "Unknown elf type: %d", et);
    return "UNKNOWN ELF Type";
  }

  static const std::string elf_type_str[] = {
      [ET_NONE] = "None",       [ET_REL] = "Relocatable File",
      [ET_EXEC] = "Executable", [ET_DYN] = "Share Object File",
      [ET_CORE] = "Core File",
  };
  return elf_type_str[et];
}

std::string elf_parser::get_machine_str(unsigned em) {
  if (em >= EM_NUM) {
    fprintf(stderr, "Unknown machine: %d", em);
    return "Unknown Machine Type";
  }

  static const std::string elf_machine_str[] = {
      [EM_NONE] = "NONE",
      [EM_M32] = "M32",
      [EM_SPARC] = "SPARC",
      [EM_386] = "386",
      [EM_68K] = "68K",
      [EM_88K] = "88K",
      [EM_860] = "860",
      [EM_MIPS] = "MIPS",
      [EM_S370] = "S370",
      [EM_MIPS_RS3_LE] = "MIPS_RS3_LE",
      [EM_PARISC] = "PARISC",
      [EM_VPP500] = "VPP500",
      [EM_SPARC32PLUS] = "SPARC32PLUS",
      [EM_960] = "960",
      [EM_PPC] = "PPC",
      [EM_PPC64] = "PPC64",
      [EM_S390] = "S390",
      [EM_V800] = "V800",
      [EM_FR20] = "FR20",
      [EM_RH32] = "RH32",
      [EM_RCE] = "RCE",
      [EM_ARM] = "ARM",
      [EM_FAKE_ALPHA] = "FAKE_ALPHA",
      [EM_SH] = "SH",
      [EM_SPARCV9] = "SPARCV9",
      [EM_TRICORE] = "TRICORE",
      [EM_ARC] = "ARC",
      [EM_H8_300] = "H8_300",
      [EM_H8_300H] = "H8_300H",
      [EM_H8S] = "H8S",
      [EM_H8_500] = "H8_500",
      [EM_IA_64] = "IA_64",
      [EM_MIPS_X] = "MIPS_X",
      [EM_COLDFIRE] = "COLDFIRE",
      [EM_68HC12] = "68HC12",
      [EM_MMA] = "MMA",
      [EM_PCP] = "PCP",
      [EM_NCPU] = "NCPU",
      [EM_NDR1] = "NDR1",
      [EM_STARCORE] = "STARCORE",
      [EM_ME16] = "ME16",
      [EM_ST100] = "ST100",
      [EM_TINYJ] = "TINYJ",
      [EM_X86_64] = "X86_64",
      [EM_PDSP] = "PDSP",
      [EM_FX66] = "FX66",
      [EM_ST9PLUS] = "ST9PLUS",
      [EM_ST7] = "ST7",
      [EM_68HC16] = "68HC16",
      [EM_68HC11] = "68HC11",
      [EM_68HC08] = "68HC08",
      [EM_68HC05] = "68HC05",
      [EM_SVX] = "SVX",
      [EM_ST19] = "ST19",
      [EM_VAX] = "VAX",
      [EM_CRIS] = "CRIS",
      [EM_JAVELIN] = "JAVELIN",
      [EM_FIREPATH] = "FIREPATH",
      [EM_ZSP] = "ZSP",
      [EM_MMIX] = "MMIX",
      [EM_HUANY] = "HUANY",
      [EM_PRISM] = "PRISM",
      [EM_AVR] = "AVR",
      [EM_FR30] = "FR30",
      [EM_D10V] = "D10V",
      [EM_D30V] = "D30V",
      [EM_V850] = "V850",
      [EM_M32R] = "M32R",
      [EM_MN10300] = "MN10300",
      [EM_MN10200] = "MN10200",
      [EM_PJ] = "P",
      [EM_OPENRISC] = "OR1K/OPENRISC",
      [EM_ARC_COMPACT] = "ARC_A5/COMPACT",
      [EM_XTENSA] = "XTENSA",
      [EM_VIDEOCORE] = "VIDEOCORE",
      [EM_TMM_GPP] = "TMM_GPP",
      [EM_NS32K] = "NS32K",
      [EM_TPC] = "TPC",
      [EM_SNP1K] = "SNP1K",
      [EM_ST200] = "ST200",
      [EM_IP2K] = "IP2K",
      [EM_MAX] = "MAX",
      [EM_CR] = "CR",
      [EM_F2MC16] = "F2MC16",
      [EM_MSP430] = "MSP430",
      [EM_BLACKFIN] = "BLACKFIN",
      [EM_SE_C33] = "SE_C33",
      [EM_SEP] = "SEP",
      [EM_ARCA] = "ARCA",
      [EM_UNICORE] = "UNICORE",
      [EM_EXCESS] = "EXCESS",
      [EM_DXP] = "DXP",
      [EM_ALTERA_NIOS2] = "ALTERA_NIOS2",
      [EM_CRX] = "CRX",
      [EM_XGATE] = "XGATE",
      [EM_C166] = "C166",
      [EM_M16C] = "M16C",
      [EM_DSPIC30F] = "DSPIC30F",
      [EM_CE] = "CE",
      [EM_M32C] = "M32C",
      [EM_TSK3000] = "",
      [EM_RS08] = "",
      [EM_SHARC] = "",
      [EM_ECOG2] = "",
      [EM_SCORE7] = "",
      [EM_DSP24] = "",
      [EM_VIDEOCORE3] = "",
      [EM_LATTICEMICO32] = "",
      [EM_SE_C17] = "",
      [EM_TI_C6000] = "",
      [EM_TI_C2000] = "",
      [EM_TI_C5500] = "",
      [EM_TI_ARP32] = "",
      [EM_TI_PRU] = "",     // 144
      [EM_MMDSP_PLUS] = "", // 160
      [EM_CYPRESS_M8C] = "",
      [EM_R32C] = "",
      [EM_TRIMEDIA] = "",
      [EM_QDSP6] = "",
      [EM_8051] = "",
      [EM_STXP7X] = "",
      [EM_NDS32] = "",
      [EM_ECOG1X] = "",
      [EM_MAXQ30] = "",
      [EM_XIMO16] = "",
      [EM_MANIK] = "",
      [EM_CRAYNV2] = "",
      [EM_RX] = "",
      [EM_METAG] = "",
      [EM_MCST_ELBRUS] = "",
      [EM_ECOG16] = "",
      [EM_CR16] = "",
      [EM_ETPU] = "",
      [EM_SLE9X] = "",
      [EM_L10M] = "",
      [EM_K10M] = "",
      [EM_AARCH64] = "",
      [EM_AVR32] = "",
      [EM_STM8] = "",
      [EM_TILE64] = "",
      [EM_TILEPRO] = "",
      [EM_MICROBLAZE] = "",
      [EM_CUDA] = "",
      [EM_TILEGX] = "",
      [EM_CLOUDSHIELD] = "",
      [EM_COREA_1ST] = "",
      [EM_COREA_2ND] = "",
      [EM_ARC_COMPACT2] = "",
      [EM_OPEN8] = "",
      [EM_RL78] = "",
      [EM_VIDEOCORE5] = "",
      [EM_78KOR] = "",
      [EM_56800EX] = "",
      [EM_BA1] = "",
      [EM_BA2] = "",
      [EM_XCORE] = "",
      [EM_MCHP_PIC] = "",
      [EM_KM32] = "",
      [EM_KMX32] = "",
      [EM_EMX16] = "",
      [EM_EMX8] = "",
      [EM_KVARC] = "",
      [EM_CDP] = "",
      [EM_COGE] = "",
      [EM_COOL] = "",
      [EM_NORC] = "",
      [EM_CSR_KALIMBA] = "",
      [EM_Z80] = "",
      [EM_VISIUM] = "",
      [EM_FT32] = "",
      [EM_MOXIE] = "",
      [EM_AMDGPU] = "",
      [EM_RISCV] = "",
      [EM_BPF] = "",
  };
  return elf_machine_str[em];
}

std::string elf_parser::get_section_type(unsigned tt) {
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

std::string elf_parser::get_segment_type(uint32_t &seg_type) {
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

std::string elf_parser::get_segment_flags(uint32_t &seg_flags) {
  std::string flags;

  if (seg_flags & PF_R) {
    flags.append("r");
  } else {
    flags.append("-");
  }

  if (seg_flags & PF_W) {
    flags.append("w");
  } else {
    flags.append("-");
  }

  if (seg_flags & PF_X) {
    flags.append("x");
  } else {
    flags.append("-");
  }

  return flags;
}

std::string elf_parser::get_symbol_type(uint8_t &sym_type) {
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

std::string elf_parser::get_symbol_bind(uint8_t &sym_bind) {
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

std::string elf_parser::get_symbol_visibility(uint8_t &sym_vis) {
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

std::string elf_parser::get_symbol_index(uint16_t &sym_idx) {
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

std::string elf_parser::get_relocation_type(uint32_t &rela_type) {
  switch (ELF32_R_TYPE(rela_type)) {
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

std::string elf_parser::get_relocation_type(uint64_t &rela_type) {
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

std::uintptr_t elf_parser::get_rel_symbol_value(uint32_t &sym_idx,
                                                std::vector<symbol_t> &syms) {
  std::uintptr_t sym_val = 0;
  for (auto &sym : syms) {
    if (sym.symbol_num == ELF32_R_SYM(sym_idx)) {
      sym_val = sym.symbol_value;
      break;
    }
  }
  return sym_val;
}

std::uintptr_t elf_parser::get_rel_symbol_value(uint64_t &sym_idx,
                                                std::vector<symbol_t> &syms) {
  std::uintptr_t sym_val = 0;
  for (auto &sym : syms) {
    if (sym.symbol_num == ELF64_R_SYM(sym_idx)) {
      sym_val = sym.symbol_value;
      break;
    }
  }
  return sym_val;
}

std::string elf_parser::get_rel_symbol_name(uint32_t &sym_idx,
                                            std::vector<symbol_t> &syms) {
  std::string sym_name;
  for (auto &sym : syms) {
    if (sym.symbol_num == ELF32_R_SYM(sym_idx)) {
      sym_name = sym.symbol_name;
      break;
    }
  }
  return sym_name;
}

std::string elf_parser::get_rel_symbol_name(uint64_t &sym_idx,
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
