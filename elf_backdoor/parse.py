'''
[ELF Document](https://uclibc.org/docs/elf-64-gen.pdf)
'''
from utils import parse_args, debug_print
from struct import unpack
from typing import List, Dict, BinaryIO, Union, Any


Header = Dict[str, Any]
Segments = List[Dict[str, int]]
Sections = List[Dict[str, Union[int, str]]]


class elf_info:
    '''
    This class will parse ELF file and get information from ELF file.
    '''

    def __init__(self, file: BinaryIO):
        self.file = file
        self.header = self._parse_header()
        self.segments = self._parse_segment()
        self.sections = self._parse_section()
        self._add_section_name()

    def _parse_header(self) -> Header:
        '''
        header parse part.
        '''
        hdr = {}
        hdr['e_ident'] = unpack('4sccccc6sc', self.file.read(16))
        hdr['e_type'], hdr['e_machine'] = unpack('<hh', self.file.read(4))
        hdr['e_version'], = unpack('<I', self.file.read(4))

        # Entry point virtual address
        hdr['e_entry'], = unpack('<Q', self.file.read(8))

        # segment/section header file offset
        hdr['e_phoff'], hdr['e_shoff'] = unpack('<QQ', self.file.read(16))

        # Processor-specific flags, ELF Header size in bytes
        hdr['e_flags'], hdr['e_ehsize'] = unpack('<Ih', self.file.read(6))

        # Program header table entry size, Program header table entry count
        hdr['e_phentsize'], hdr['e_phnum'] = unpack('<hh', self.file.read(4))

        # Section header table entry size, Section header table entry count
        hdr['e_shentsize'], hdr['e_shnum'] = unpack('<hh', self.file.read(4))

        # Section header string table index
        hdr['e_shtrndx'], = unpack('<h', self.file.read(2))

        return hdr

    def _parse_segment(self) -> Segments:
        '''
        segment/program header parse part.
        '''
        self.file.seek(self.header['e_phoff'])
        segments = []
        for i in range(self.header['e_phnum']):
            seg = {}
            # Type of segment, Segment attributes
            seg['p_type'], seg['p_flags'] = unpack('<II', self.file.read(8))

            # Offset in file
            seg['p_offset'], = unpack('<Q', self.file.read(8))

            # Virtual address in memory, Reserved
            seg['p_vaddr'], seg['p_paddr'] = unpack('<QQ', self.file.read(16))

            # Size of segment in file, Size of segment in memory
            seg['p_filesz'], seg['p_memsz'] = unpack('<QQ', self.file.read(16))

            # Alignment of segment
            seg['p_align'], = unpack('<Q', self.file.read(8))

            segments.append(seg)
        return segments

    def _add_section_name(self):
        '''
        After section parsing,
        this class will use ELF header's e_shtrndx to get shstrtab's position
        and get other sections' name.
        '''
        # use session string index to get session string's file offset
        shstrtab_offset = self.sections[self.header['e_shtrndx']]['sh_offset']
        for j in self.sections:
            # use file offset to get section name
            assert isinstance(shstrtab_offset, int) and \
                isinstance(j['sh_name'], int)
            self.file.seek(shstrtab_offset + j['sh_name'])
            s_name_str = ''
            while True:
                t = self.file.read(1)
                if t == b'\x00':
                    break
                s_name_str += t.decode()
            j['s_name_str'] = s_name_str

    def _parse_section(self) -> Sections:
        '''
        section parse part.
        '''
        self.file.seek(self.header['e_shoff'])
        sections = []
        for i in range(self.header['e_shnum']):
            sec = {}
            # Section name and type
            sec['sh_name'], sec['sh_type'] = unpack('<II', self.file.read(8))

            # Section attributes
            sec['sh_flags'], = unpack('<Q', self.file.read(8))

            # Virtual address in memory, Offset in file
            sec['sh_addr'], sec['sh_offset'] = unpack('QQ', self.file.read(16))

            # Size of section
            sec['sh_size'], = unpack('<Q', self.file.read(8))

            # Link to other section Miscellaneous information
            sec['sh_link'], sec['sh_info'] = unpack('<II', self.file.read(8))

            # Address alignment boundary, Size of entries, if section has table
            sec['sh_addralign'], sec['sh_entsize'] = unpack(
                'QQ', self.file.read(16))

            sections.append(sec)
        return sections

    def analyse_segment(self) -> Dict[int, List[str]]:
        '''
        Analyse the sessions and segment's relation
        '''
        dic: Dict[int, List[str]] = {}
        for i in range(len(self.segments)):
            dic[i] = []
        for sec in self.sections:
            assert isinstance(sec['sh_addr'], int) and \
                isinstance(sec['sh_size'], int)
            sec_begin = sec['sh_addr']
            sec_end = sec_begin + sec['sh_size']
            for i, seg in enumerate(self.segments):
                seg_begin = seg['p_vaddr']
                seg_end = seg_begin + seg['p_memsz']
                if sec_begin >= seg_begin and sec_end <= seg_end:
                    assert isinstance(sec['s_name_str'], str)
                    dic[i].append(sec['s_name_str'])
        return dic


if __name__ == '__main__':
    args = parse_args()
    with open(args.file, 'rb') as f:
        info = elf_info(f)
        result = info.analyse_segment()
        debug_print(info.header,
                    info.segments,
                    info.sections,
                    result)
