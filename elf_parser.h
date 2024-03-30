// MIT License

// Copyright (c) 2018 finixbit

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cstdio>
#include <cstdlib>
#include <fcntl.h> /* O_RDONLY */
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/mman.h> /* mmap, MAP_PRIVATE */
#include <sys/stat.h> /* For the size of the file. , fstat */
#include <vector>

#include "elf.h" // Elf64_Shdr

namespace elf_parser {

typedef struct section {
  int section_index = 0;
  std::uintptr_t section_offset;
  std::uintptr_t section_addr;
  std::string section_name;
  std::string section_type;
  size_t section_size;
  size_t section_ent_size;
  uint64_t section_addr_align;
} section_t;

typedef struct segment {
  std::string segment_type;
  std::string segment_flags;
  uint64_t segment_offset;
  uint64_t segment_virtaddr;
  uint64_t segment_physaddr;
  uint64_t segment_filesize;
  uint64_t segment_memsize;
  uint64_t segment_align;
} segment_t;

typedef struct symbol {
  std::string symbol_index;
  std::uintptr_t symbol_value;
  uint64_t symbol_num = 0;
  uint64_t symbol_size = 0;
  std::string symbol_type;
  std::string symbol_bind;
  std::string symbol_visibility;
  std::string symbol_name;
  std::string symbol_section;
} symbol_t;

typedef struct relocation {
  std::uintptr_t relocation_offset;
  std::uintptr_t relocation_info;
  std::uintptr_t relocation_symbol_value;
  std::string relocation_type;
  std::string relocation_symbol_name;
  std::string relocation_section_name;
  std::uintptr_t relocation_plt_address;
} relocation_t;

std::string get_e_type_str(unsigned et);
std::string get_machine_str(unsigned em);
std::string get_section_type(unsigned tt);

std::string get_segment_type(uint32_t &seg_type);
std::string get_segment_flags(uint32_t &seg_flags);

std::string get_symbol_type(uint8_t &sym_type);
std::string get_symbol_bind(uint8_t &sym_bind);
std::string get_symbol_visibility(uint8_t &sym_vis);
std::string get_symbol_index(uint16_t &sym_idx);

std::string get_relocation_type(uint32_t &rela_type);
std::string get_relocation_type(uint64_t &rela_type);
std::uintptr_t get_rel_symbol_value(uint32_t &sym_idx,
                                    std::vector<symbol_t> &syms);
std::uintptr_t get_rel_symbol_value(uint64_t &sym_idx,
                                    std::vector<symbol_t> &syms);
std::string get_rel_symbol_name(uint32_t &sym_idx, std::vector<symbol_t> &syms);
std::string get_rel_symbol_name(uint64_t &sym_idx, std::vector<symbol_t> &syms);

class Elf32 {
public:
  Elf32(uint8_t *m_mmap_program);
  std::vector<section_t> get_sections();
  std::vector<segment_t> get_segments();
  std::vector<symbol_t> get_symbols();
  std::vector<relocation_t> get_relocations();

  void print_header();
  void print_section_headers();
  void print_program_hdrs();
  Elf32_Ehdr *get_header();

protected:
  uint8_t *m_mmap_program;

  void print_section_header(section_t &sec);
  void print_program_hdr(segment_t &seg);
};

class Elf_parser {
public:
  Elf_parser(std::string &program_path);

  void print_header();
  void print_section_headers();
  void print_program_hdrs();

private:
  uint8_t *load_memory_map();
  unsigned verify_identity(Elf64_Ehdr *header);

  Elf32 *elf32;
  std::string m_program_path;
};
}; // namespace elf_parser
