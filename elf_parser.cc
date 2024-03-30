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

#include "elf_parser.h"
using namespace elf_parser;

Elf_parser::Elf_parser(std::string &program_path)
    : m_program_path{program_path} {
  uint8_t *m_program = load_memory_map();
  unsigned ei_class = verify_identity((Elf64_Ehdr *)m_program);
  switch (ei_class) {
  case ELFCLASS32:
    printf("ELF 32-bit file\n");
    elf32 = new Elf32{m_program};
    break;
  case ELFCLASS64:
    printf("ELF 64-bit file, NOT SUPPORTED.\n");
    break;
  case ELFCLASSNONE:
  default:
    fprintf(stderr, "Unexpected EFL Class ID: %d.\n", ei_class);
    break;
  }
}

unsigned Elf_parser::verify_identity(Elf64_Ehdr *header) {
  if (memcmp(header->e_ident, ELFMAG, SELFMAG)) {
    printf("Header Magic mismatch(%02x%02x%02x%02x).\n", header->e_ident[0],
           header->e_ident[1], header->e_ident[2], header->e_ident[3]);
    return ELFCLASSNONE;
  }
  return header->e_ident[EI_CLASS];
}

uint8_t *Elf_parser::load_memory_map() {
  int fd;
  struct stat st;

  if ((fd = open(m_program_path.c_str(), O_RDONLY)) < 0) {
    printf("Err: open\n");
    exit(-1);
  }
  if (fstat(fd, &st) < 0) {
    printf("Err: fstat\n");
    exit(-1);
  }
  uint8_t *m_mmap_program = static_cast<uint8_t *>(
      mmap(NULL, (size_t)st.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
  if (m_mmap_program == MAP_FAILED) {
    printf("Err: mmap\n");
    exit(-1);
  }
  return m_mmap_program;
}

void Elf_parser::print_header() { elf32->print_header(); }

void Elf_parser::print_section_headers() { elf32->print_section_headers(); }
void Elf_parser::print_program_hdrs() { elf32->print_program_hdrs(); }
