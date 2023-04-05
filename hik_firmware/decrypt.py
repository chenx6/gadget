from io import SEEK_SET, SEEK_CUR
from os import mkdir
from typing import BinaryIO
from struct import unpack
from logging import debug, info, INFO, basicConfig, warning, DEBUG
from argparse import ArgumentParser

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad

xor_key = b"\xba\xcd\xbc\xfe\xd6\xca\xdd\xd3\xba\xb9\xa3\xab\xbf\xcb\xb5\xbe"
# fmt: off
aes_keys = {
    0x780000: [
        0xB6, 0x4E, 0xC5, 0x08, 0x66, 0xC4, 0x9B, 0x75,
        0x7F, 0x1B, 0x27, 0x26, 0xA3, 0x75, 0x5F, 0x22,
        0x52, 0xAB, 0xDF, 0xE9, 0xFB, 0xBB, 0x15, 0x1E,
        0x24, 0x7D, 0x0D, 0x80, 0x6A, 0xE4, 0x25, 0xDB,
    ], 
    0x780001: [
        0xBD, 0xDC, 0x07, 0x0B, 0x81, 0xFC, 0x4E, 0x47,
        0xD0, 0x3A, 0x7E, 0xD0, 0x4C, 0x5B, 0x58, 0xD0,
        0x72, 0xE7, 0x21, 0x5D, 0x4A, 0xD4, 0x63, 0x47,
        0x54, 0xB2, 0xE5, 0xB6, 0x87, 0x32, 0xBD, 0x37,
    ],
}
# fmt: on


def hexdump(b: bytes):
    """Debug use"""
    header = ""
    for i in range(16):
        header += f"{i: 2x} "
    header += " "
    for i in range(16):
        header += f"{i:x}"
    debug(header)
    for i in range(0, len(b), 16):
        silced = b[i : i + 16]
        fmted = ""
        asciied = ""
        for i in silced:
            fmted += f"{i:02x} "
            ch = chr(i)
            if ch.isalnum():
                asciied += ch
            else:
                asciied += " "
        if len(silced) != 16:
            fmted = fmted.ljust(3 * 16, " ")
        debug("%s%s", fmted, asciied)


def decode_xor16(buf: bytes, key: bytes, length: int) -> bytes:
    result = bytearray()
    for index in range(length):
        key_byte = key[index + (index >> 4) & 0xF]
        result.append(key_byte ^ buf[index])
    return bytes(result)


def gen_aes_key(key: bytes, length: int):
    key_gen = bytearray()
    for i in range(length):
        ch = key[i]
        v5 = (
            ((i * i) & 0xFF)
            + ((ch * ch) & 0xFF)
            + ((ch % length) & 0xFF)
            + ((ch * length * i) & 0xFF)
        )
        key_gen.append((v5 & 0xFF) ^ ch)
    return bytes(key_gen)


def parse_common_header(f: BinaryIO):
    """Parse common header start"""
    header = f.read(16)
    header = decode_xor16(header, xor_key, 16)
    (header_magic, header_checksum, header_size, file_count) = unpack("<IIII", header)
    info(
        f"magic: {header_magic:x} checksum: {header_checksum:x} size: {header_size} count: {file_count}"
    )
    return header_magic, header_checksum, header_size, file_count


def parse_file_list(data: bytes):
    """Parse file list struct in header data"""
    name = data[:0x20]
    name = name[: name.index(b"\x00")].decode()
    offset, length, checksum = unpack("<III", data[0x20:0x2C])
    info(f"name: {name} off: {offset:x} length: {length} checksum: {checksum:x}")
    return name, offset, length, checksum


def dump_file(f: BinaryIO, firm_flg: int, offset: int, length: int) -> bytes:
    """Dump file with AES Key, offset and length"""
    f.seek(offset, SEEK_SET)
    cipher = f.read(length)
    if firm_flg != 0:
        key = gen_aes_key(bytes(aes_keys[firm_flg]), 32)
        aes = AES.new(key, AES.MODE_ECB)
        plaintext = aes.decrypt(pad(cipher, 32))
        return plaintext
    else:
        return cipher


def save_to_folder(name: str, data: bytes, folder: str):
    with open(f"{folder}/{name}", "wb") as f:
        f.write(data)


def parse_hk20(f: BinaryIO):
    """Parse header starts with 02KH"""
    header_magic, header_checksum, header_size, file_count = parse_common_header(f)
    f.seek(-16, SEEK_CUR)
    header = f.read(header_size)
    header = decode_xor16(header, xor_key, header_size)
    hexdump(header[:0x40])


def parse_hk30(
    f: BinaryIO, header_key: int, file_key: int, save_folder: str | None = None
):
    """Parse header starts with "03KH" """
    curr_offset = f.tell()
    header_magic, header_checksum, header_size, file_count = parse_common_header(f)
    body = f.read(header_size)
    key = gen_aes_key(bytes(aes_keys[header_key]), 32)
    aes = AES.new(key, AES.MODE_ECB)
    body = aes.decrypt(pad(body, 32))
    hexdump(body[:0x40])
    # XXX: hard coded 0XF0 offset
    for idx, off in enumerate(range(0xF0, len(body), 0x40)):
        if idx >= file_count:
            break
        name, offset, length, checksum = parse_file_list(body[off : off + 0x2C])
        dumped = dump_file(f, file_key, offset + curr_offset, length)
        hexdump(dumped[:0x20])
        if save_folder:
            save_to_folder(name, dumped, save_folder)


def parse_hkws(f: BinaryIO, save_folder: str | None = None):
    """Parse header starts with "" """
    header_magic, header_checksum, header_size, file_count = parse_common_header(f)
    f.seek(-16, SEEK_CUR)
    header = f.read(header_size)
    header = decode_xor16(header, xor_key, header_size)
    hexdump(header[:0x40])
    for off in range(0x40, header_size, 0x2C):
        name, offset, length, checksum = parse_file_list(header[off : off + 0x2C])
        dumped = dump_file(f, 0, offset, length)
        hexdump(dumped[:0x20])
        if save_folder:
            save_to_folder(name, dumped, save_folder)


parser = ArgumentParser()
parser.add_argument("-f", "--file", help="digicap.dav file")
parser.add_argument("-v", "--verbose", action="count", default=0)
parser.add_argument("-t", "--type", help="device type")
parser.add_argument("-o", "--output", help="output folder", required=False)
args = parser.parse_args()
match args.verbose:
    case 1:
        basicConfig(level=INFO)
    case 2:
        basicConfig(level=DEBUG)
match args.type:
    case "r6":
        header_key, file_key = 0x780000, 0x780001
    case _:
        header_key, file_key = 0, 0
if folder := args.output:
    mkdir(folder)
f = open(args.file, "rb")
while True:
    next_header = f.read(16)
    if len(next_header) != 16:
        break
    next_header = decode_xor16(next_header, xor_key, 16)
    f.seek(-16, SEEK_CUR)
    # Decode header and file based on magic number
    match int.from_bytes(next_header[:4], "little"):
        case 0x484B5753:
            parse_hkws(f, args.output)
        case 0x484B3230:
            parse_hk20(f)
        case 0x484B3330:
            if header_key == 0:
                warning("header_key undefined")
            parse_hk30(f, header_key, file_key, args.output)
        case _:
            break
