from enum import Enum
from typing import Any, NamedTuple, Optional
from re import sub, search
from tempfile import NamedTemporaryFile


import gdb

previous_registers: Optional[dict[str, int]] = None
call_conventions = {"i386:x86-64": ["rdi", "rsi", "rdx", "r10", "r8", "r9"], "i386": []}


class AddrRange(NamedTuple):
    start_addr: int
    end_addr: int
    permission: str


class XrefType(Enum):
    string = 1
    addr = 2
    data = 3


class Xref(NamedTuple):
    addr: int
    type_: XrefType
    data: Optional[bytes] = None
    next_xref: Optional["Xref"] = None


class Panel:
    """
    Hand-written panel for better printing columns
    """

    def __init__(self):
        self.lines: list[str] = []
        self.next_idx = 0
        self.max_width = 0

    def add_line(self, line: str):
        self.lines.append(line)
        self.max_width = max(len(line), self.max_width)

    def next_line(self) -> Optional[str]:
        if self.next_idx >= len(self.lines):
            return None
        ret = self.lines[self.next_idx]
        self.next_idx += 1
        return ret.ljust(self.max_width, " ")


class GGDDBB:
    """
    Main logic

    get_* function is used to get information, show_* function is used to generate `Panel`
    """

    def __init__(self):
        self.frame = gdb.selected_frame()
        self.arch = self.frame.architecture()
        self.register_groups = [i.name for i in self.arch.register_groups()]
        self.registers = [reg.name for reg in self.arch.registers("general")]
        ip = filter(lambda x: "ip" in x, self.registers)
        self.ip = next(ip)
        self.inf = gdb.selected_inferior()
        self.addr_ranges = self.parse_map()

    def get_registers(self) -> dict[str, int]:
        rs: dict[str, int] = {}
        for r in self.registers:
            value = self.frame.read_register(r)
            value = int(value)
            if value < 0:
                value = value & (2 ** value.bit_length() - 1)
            rs[r] = value
        return rs

    def show_registers(
        self, rs: dict[str, int], prev_rs: Optional[dict[str, int]] = None
    ) -> Panel:
        panel = Panel()
        for r, v in rs.items():
            value = hex(v)
            value = value.rjust(15, " ")
            # if this register's value changed, highlight it
            if prev_rs and prev_rs[r] != v:
                value = f"\x1b[1;31m{value}\x1b[1;0m"
            register = r.ljust(7, " ")
            panel.add_line(f"{register} {value}")
        return panel

    def get_disassemble(self, count: int) -> list[dict[str, Any]]:
        ip_val = self.frame.read_register(self.ip)
        return self.arch.disassemble(int(ip_val), count=count)

    def show_disassembly(self, disasm: list[dict[str, Any]]) -> Panel:
        panel = Panel()
        show_arrow = False
        for d in disasm:
            asm = str(d["asm"])
            asm = sub("<.+>", "", asm)
            asm = sub(" +#", "", asm)
            addr = hex(int(d["addr"])).rjust(17, " ")
            if not show_arrow:
                addr = "=> " + addr[3:]
                show_arrow = True
            panel.add_line(f"{addr} {asm}")
        return panel

    def parse_map(self) -> list[AddrRange]:
        """
        Parse /proc/self/maps to get process's address space
        """
        addr_ranges = []
        try:
            maps = open(f"/proc/{self.inf.pid}/maps", "r")
        except Exception:
            return addr_ranges
        for l in maps.readlines():
            # 55e60475c000-55e60475e000 r--p 00000000 103:02 1611108                   /usr/bin/cat
            r = search(r"([\da-f]+)-([\da-f]+) ([\w\-]+) \d+ [\d:]+ \d+ [\w\W]+", l)
            if not r:
                continue
            start_addr, end_addr, perm = r.groups()
            addr_ranges.append(AddrRange(int(start_addr, 16), int(end_addr, 16), perm))
        return addr_ranges

    def in_addr_range(self, addr: int) -> bool:
        for r in self.addr_ranges:
            if r.start_addr <= addr and r.end_addr >= addr:
                return True
        return False

    def can_xref(self):
        return len(self.addr_ranges) != 0

    def get_xref(self, addr: int, depth: int = 0) -> Xref:
        """
        Get address references to
        """
        if depth >= 3:
            return Xref(addr, XrefType.data, None)
        curr_ty = XrefType.data
        curr_addr = addr
        opt_xref_next: Optional[Xref] = None
        opt_data: Optional[bytes] = None
        if self.in_addr_range(curr_addr):
            mem = self.inf.read_memory(curr_addr, 8)
            mem = mem.tobytes()
            if mem.isascii():
                curr_ty = XrefType.string
            curr_addr = int.from_bytes(mem, "little")
            if self.in_addr_range(curr_addr):
                opt_xref_next = self.get_xref(curr_addr, depth + 1)
                curr_ty = XrefType.addr
            else:
                opt_data = mem
                curr_ty = XrefType.data
        return Xref(addr, curr_ty, opt_data, opt_xref_next)

    def show_xref(self, hint: str, xref: Xref) -> Panel:
        panel = Panel()
        curr_xref: Optional[Xref] = xref
        texts: list[str] = []
        while curr_xref != None:
            if curr_xref.type_ == XrefType.addr and curr_xref.next_xref:
                texts.append(f"0x{curr_xref.next_xref.addr:x}")
            elif curr_xref.type_ == XrefType.string and curr_xref.data:
                texts.append(f'"{curr_xref.data.decode()}"')
            else:
                texts.append(str(curr_xref.data))
            curr_xref = curr_xref.next_xref
        line = f"{hint}: 0x{xref.addr:x} => {' => '.join(texts)}"
        panel.add_line(line)
        return panel

    def call_analysis(self) -> dict[str, int]:
        """TODO: SUIT FOR MORE ARCHITECTURE"""
        conv = call_conventions[self.arch.name()]
        regs = self.get_registers()
        ret = {}
        for c in conv:
            ret[c] = regs[c]
        return ret

    def print_columns(self, panels: list[Panel]):
        pl = len(panels)
        while True:
            none_cnt = 0
            cols = []
            for idx, p in enumerate(panels):
                l = p.next_line()
                if not l:
                    cols.append(" " * p.max_width)
                    none_cnt += 1
                else:
                    cols.append(l)
                if idx == pl - 1:
                    print(" ".join(cols))
            if none_cnt == pl:
                break


def run():
    try:
        g = GGDDBB()
    except gdb.error as e:
        print(e)
    else:
        global previous_registers
        r = g.get_registers()
        pr = g.show_registers(r, previous_registers)
        d = g.get_disassemble(12)
        pd = g.show_disassembly(d)
        g.print_columns([pd, pr])
        if previous_registers:
            for reg, v in r.items():
                if "ip" in reg or "flag" in reg:
                    continue
                prev_v = previous_registers[reg]
                if prev_v == v:
                    continue
                if not g.can_xref():
                    continue
                xref = g.get_xref(v)
                px = g.show_xref(reg, xref)
                g.print_columns([px])
        previous_registers = r


def add_hook():
    temp = NamedTemporaryFile()
    temp.write(
        b"""
    define hook-stop
        python run()
    end
"""
    )
    temp.flush()
    gdb.execute(f"source {temp.name}")
    temp.close()


run()
add_hook()
