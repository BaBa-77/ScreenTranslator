#include "BREWRuntimePatcher.h"
#include <elfio/elfio.hpp>

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Usage: BREWRuntimePatcher <elf_file> <out_file>" << std::endl;
		return 1;
	}

	ELFIO::elfio elf;

	if (!elf.load(argv[1])) {
		std::cout << "Can't find or process ELF file " << argv[1] << std::endl;
		return 2;
	}

	ELFIO::Elf_Xword numSections = elf.sections.size();

	for (ELFIO::Elf_Xword i = 0; i < numSections; i++) {
		ELFIO::section* sec = elf.sections[i];

		if ( SHT_SYMTAB == sec->get_type() ||
			 SHT_DYNSYM == sec->get_type()) {
			ELFIO::symbol_section_accessor symbols(elf, sec);


			const char* pDataC = sec->get_data();
			ELFIO::Elf_Xword sectionSize = sec->get_size();
			char* pData = (char*) new char[sectionSize];
			memcpy(pData, pDataC, sectionSize);

			int numSymbols = symbols.get_symbols_num();
			for (int i = 0; i < numSymbols; i++) {
				ELFIO::Elf32_Sym* pSym = reinterpret_cast<ELFIO::Elf32_Sym*>(
					pData +
					i * sec->get_entry_size());
				const ELFIO::endianess_convertor& convertor = elf.get_convertor();
				
				ELFIO::Elf_Half shndx = convertor(pSym->st_shndx);
				if (shndx == SHN_UNDEF) {
					unsigned char st_other = pSym->st_other;
					st_other &= ~0x3;
					st_other |= STV_DEFAULT;
				}
			}

			sec->set_data(pData, sectionSize);

			delete[] pData;
		}
	}

	elf.save(argv[2]);

	return 0;
}
