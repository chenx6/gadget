from argparse import ArgumentParser
from typing import Callable
from curses import (
    wrapper,
    curs_set,
    KEY_DOWN,
    KEY_UP,
    KEY_BACKSPACE,
    KEY_NPAGE,
    KEY_PPAGE,
    error,
    window,
    init_pair,
    color_pair,
    COLOR_BLUE,
    COLOR_BLACK,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_WHITE,
)
from enum import IntEnum
from io import BufferedReader
from logging import getLogger, FileHandler, DEBUG, INFO


class Color(IntEnum):
    BLUE = 1
    GREEN = 2
    YELLOW = 3
    GREY = 4
    WHITE = 5


class Mode(IntEnum):
    Normal = 1
    Goto = 2
    Search = 3


class ProgramStatus(IntEnum):
    Exit = 1
    ShowCursor = 2
    HideCursor = 3


def get_logger(debug: bool = False):
    level = DEBUG if debug else INFO
    logger = getLogger("hv")
    logger.setLevel(level)
    if debug:
        handler = FileHandler("hv.log")
        handler.setLevel(level)
        logger.addHandler(handler)
    return logger


def get_color(ch: int) -> Color:
    if ch == 0:
        # NULL
        return Color.GREY
    elif 0x20 < ch <= 0x7E:
        # ASCII printable
        return Color.BLUE
    elif 0x80 <= ch < 0xFF:
        # Non-ASCII
        return Color.YELLOW
    else:
        # Whitespace/Control character
        return Color.GREEN


def search_pattern(file: BufferedReader, pattern: bytes, buf_size: int = 1024 * 1024):
    if not pattern:
        return -1
    pre_pos = file.tell()
    total = 0  # Total read length
    overlap_len = len(pattern) - 1  # Overlap length
    leftover = b""  # Overlap buf
    offset = -1
    while True:
        curr_chunk = file.read(buf_size)
        if not curr_chunk and not leftover:
            break
        # Concat overlap and current length
        search_buf = leftover + curr_chunk
        idx = search_buf.find(pattern)
        if idx != -1:
            offset = total - len(leftover) + idx + pre_pos
            break
        if len(curr_chunk) < buf_size:
            break
        total += len(curr_chunk)
        # Calculate overlap
        leftover = search_buf[-overlap_len:] if overlap_len > 0 else b""
    file.seek(pre_pos)
    return offset


class Variable[T]:
    def __init__(self, value: T) -> None:
        self._value = value
        self._callbacks: list[Callable] = []

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, new_val: T):
        self._value = new_val
        for cb in self._callbacks:
            cb(new_val)

    def subscribe(self, callback: Callable):
        self._callbacks.append(callback)


class HexViewerViewModel:
    def __init__(self, file_path: str, height: int) -> None:
        self._file = open(file_path, "rb")
        self.height = height - 2
        self.data = self._file.read(16 * self.height)
        self.line_offset = Variable(value=0)
        self.line_offset.subscribe(lambda n: self.goto(n))
        self.mode = Variable(Mode.Normal)
        self.cmd_buf = Variable("")
        self.status = Variable(ProgramStatus.HideCursor)

    def goto(self, new_line_offset: int):
        self._file.seek(new_line_offset * 16)
        self.data = self._file.read(16 * self.height)

    def search(self, pattern: str):
        pos = search_pattern(self._file, pattern.encode())
        if pos != -1:
            self.line_offset.value = int(pos / 16)

    def _handle_command(self, key: int):
        logger.debug("%d %s", key, self.cmd_buf.value)
        if key == 10:
            # Enter
            if self.mode.value == Mode.Goto:
                if self.cmd_buf.value.isdigit():
                    self.line_offset.value = int(self.cmd_buf.value)
            elif self.mode.value == Mode.Search:
                self.search(self.cmd_buf.value)
            self.cmd_buf.value = ""
            self.mode.value = Mode.Normal
            self.status.value = ProgramStatus.HideCursor
        elif key == 27:
            # ESC
            self.cmd_buf.value = ""
            self.mode.value = Mode.Normal
            self.status.value = ProgramStatus.HideCursor
        elif key in (KEY_BACKSPACE, 127, 8):
            # Backspace
            self.cmd_buf.value = self.cmd_buf.value[:-1]
        elif chr(key).isprintable():
            self.cmd_buf.value += chr(key)

    def handle_key(self, key: int):
        if self.mode.value in (Mode.Goto, Mode.Search):
            self._handle_command(key)
        else:
            if key == ord("q"):
                self.status.value = ProgramStatus.Exit
            elif key == ord("/"):
                self.status.value = ProgramStatus.ShowCursor
                self.mode.value = Mode.Search
            elif key == ord(":"):
                self.status.value = ProgramStatus.ShowCursor
                self.mode.value = Mode.Goto
            elif key == KEY_DOWN:
                self.line_offset.value += 1
            elif key == KEY_UP:
                if self.line_offset.value >= 1:
                    self.line_offset.value -= 1
            elif key == KEY_NPAGE or key == ord("D") & 0x1F:
                self.line_offset.value += 10
            elif key == KEY_PPAGE or key == ord("B") & 0x1F:
                if self.line_offset.value >= 10:
                    self.line_offset.value -= 10


class HexViewerView:
    HEADER = "          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f 0123456789abcdef"

    def __init__(self, stdscr: window, vm: HexViewerViewModel):
        self.stdscr = stdscr
        self.vm = vm

        self.vm.line_offset.subscribe(lambda _: self.render())
        self.vm.mode.subscribe(lambda _: self.render())
        self.vm.cmd_buf.subscribe(lambda _: self.render())
        self.vm.status.subscribe(self.handle_status)

        curs_set(0)
        stdscr.nodelay(False)
        init_pair(Color.BLUE, COLOR_BLUE, COLOR_BLACK)
        init_pair(Color.GREEN, COLOR_GREEN, COLOR_BLACK)
        init_pair(Color.YELLOW, COLOR_YELLOW, COLOR_BLACK)
        init_pair(Color.GREY, COLOR_WHITE, COLOR_BLACK)
        init_pair(Color.WHITE, COLOR_WHITE, COLOR_BLACK)
        self.render()

    def render(self):
        try:
            self._render_impl()
        except error:
            pass

    def _render_impl(self):
        self.stdscr.clear()
        data = self.vm.data
        # Render header
        self.stdscr.addstr(0, 0, self.HEADER)
        for idx, i in enumerate(range(0, len(data), 16), 1):
            # Render line
            line_data = data[i : i + 16]
            addr_offset = self.vm.line_offset.value * 16 + i

            col = 0
            # Line layout: [address] [hex value] [char value]
            addr = f"{addr_offset:08x} "
            self.stdscr.addstr(idx, col, addr)
            col += len(addr)
            for h in line_data:
                hex_text = f"{h:02x} "
                color = get_color(h)
                self.stdscr.addstr(idx, col, hex_text, color_pair(color))
                col += len(hex_text)
            for h in line_data:
                char_text = chr(h)
                if not char_text.isprintable():
                    char_text = "."
                color = get_color(h)
                self.stdscr.addstr(idx, col, char_text, color_pair(color))
                col += len(char_text)
        # Render command
        if self.vm.mode.value in (Mode.Goto, Mode.Search):
            self.stdscr.move(self.vm.height + 1, 0)
            self.stdscr.addstr(self.vm.cmd_buf.value)
        self.stdscr.refresh()

    def handle_status(self, new_status: ProgramStatus):
        match new_status:
            case ProgramStatus.HideCursor:
                curs_set(0)
            case ProgramStatus.ShowCursor:
                curs_set(1)
            case ProgramStatus.Exit:
                exit()


def main(file_path: str, stdscr: window):
    # Use MVVM architecture, Model <=> ViewModel <=> View
    height, width = stdscr.getmaxyx()
    vm = HexViewerViewModel(file_path, height)
    view = HexViewerView(stdscr, vm)  # noqa: F841
    while True:
        key = stdscr.getch()
        vm.handle_key(key)


logger = get_logger()
if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("file")
    args = parser.parse_args()
    wrapper(lambda w: main(args.file, w))
