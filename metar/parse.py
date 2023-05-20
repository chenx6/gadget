from typing import NamedTuple, Callable, Any, Optional
from functools import reduce


def flatten(list_of_lists):
    ret = []

    def outer_append(lst):
        for i in lst:
            if isinstance(i, list):
                outer_append(i)
            else:
                ret.append(i)

    outer_append(list_of_lists)
    return ret


def join(lst: list[str]):
    return "".join(lst)


class Stream(NamedTuple):
    src: str
    idx: int


class OutputValue(NamedTuple):
    stream: Stream
    value: Any


class DateExpr(NamedTuple):
    day: str
    time: str


class VisibilityExpr(NamedTuple):
    num: str


class WindExpr(NamedTuple):
    direction: str
    speed: str
    unit: str
    g: Optional[str] = None


class WindVaryExpr(NamedTuple):
    from_: str
    to: str


class TemperatureExpr(NamedTuple):
    temperature: str
    dew: str


class PressureExpr(NamedTuple):
    unit: str
    num: str


class CloudExpr(NamedTuple):
    desc: str
    num: Optional[str] = None
    cb: Optional[str] = None


Result = OutputValue | None
Parser = Callable[[Stream], Result]
Generator = Callable[[str], Parser]
Combinator = Callable[[list[Parser]], Parser]


def char(ch: str) -> Parser:
    """Consume a char"""

    def parser(s: Stream) -> Result:
        return (
            OutputValue(Stream(s.src, s.idx + 1), s.src[s.idx])
            if s.idx < len(s.src) and s.src[s.idx] == ch
            else None
        )

    return parser


def identifier(ch: str) -> Parser:
    """Consume a identifier"""
    ch_len = len(ch)

    def parser(s: Stream) -> Result:
        return (
            OutputValue(Stream(s.src, s.idx + ch_len), ch)
            if s.src[s.idx : s.idx + ch_len] == ch
            else None
        )

    return parser


def or_(p1: Parser, p2: Parser) -> Parser:
    def parser(input_: Stream) -> Result:
        return p1(input_) or p2(input_)

    return parser


def and_(p1: Parser, p2: Parser) -> Parser:
    def parser(input_: Stream) -> Result:
        if r := p1(input_):
            if r2 := p2(r.stream):
                return OutputValue(r2.stream, flatten([r.value, r2.value]))
        return None

    return parser


def one_or_more(p: Parser) -> Parser:
    """Use parser to consume one or more chars"""

    def parser(input_: Stream) -> Result:
        ret = []
        inp: Stream | None = None
        if curr := p(input_):
            inp = curr.stream
            ret.append(curr)
        else:
            return None
        while curr := p(inp):
            inp = curr.stream
            ret.append(curr)
        return OutputValue(inp, flatten([i.value for i in ret]))

    return parser


def zero_or_more(p: Parser) -> Parser:
    """Use parser to consume zero or more chars"""

    def parser(input_: Stream) -> Result:
        ret = []
        inp: Stream | None = input_
        while curr := p(inp):
            inp = curr.stream
            ret.append(curr)
        return OutputValue(inp, flatten([i.value for i in ret]))

    return parser


def n_or_more(n: int, p: Parser) -> Parser:
    def parser(input_: Stream) -> Result:
        ret = []
        inp: Stream | None = input_
        for i in range(n):
            if curr := p(inp):
                inp = curr.stream
                ret.append(curr)
            else:
                inp = None
                break
        return (
            OutputValue(inp if inp != None else input_, flatten([i.value for i in ret]))
            if inp != None
            else None
        )

    return parser


def any_(*args: Parser) -> Parser:
    return reduce(or_, args)


def and_then(*args: Parser) -> Parser:
    """Consume chars with parser in args"""
    return reduce(and_, args)


def any_char(ch: str) -> Parser:
    def parser(input_: Stream):
        return any_(*map(char, ch))(input_)

    return parser


def map_(parser: Parser, fn: Callable) -> Parser:
    def p(input_: Stream):
        if out := parser(input_):
            return OutputValue(out.stream, fn(out.value))

    return p


def digit_expr() -> Parser:
    return any_char("0123456789")


def metar_expr() -> Parser:
    metar = identifier("METAR")
    slash = char("/")
    speci = identifier("SPECI")
    return and_(metar, zero_or_more(and_(slash, speci)))


def icao_expr() -> Parser:
    upper_alphabet = any_char("".join([chr(i) for i in range(65, 91)]))
    return map_(n_or_more(4, upper_alphabet), join)


def time_expr() -> Parser:
    digit = digit_expr()
    z = char("Z")

    day = map_(n_or_more(2, digit), join)
    time = map_(n_or_more(4, digit), join)
    return map_(and_then(day, time, z), lambda v: DateExpr(v[0], v[1]))


def wind_expr() -> Parser:
    digit = digit_expr()
    mps = identifier("MPS")
    kt = identifier("KT")
    kmh = identifier("KMH")
    g = char("G")

    direc = map_(or_(n_or_more(3, digit), identifier("VRB")), join)
    speed = map_(n_or_more(2, digit), join)
    return map_(
        and_then(
            direc,
            speed,
            zero_or_more(g),
            any_(mps, kt, kmh),
        ),
        lambda v: WindExpr(v[0], v[1], v[-1], "G" if v[2] == "G" else None),
    )


def wind_vary_expr() -> Parser:
    direc = map_(n_or_more(3, digit_expr()), join)
    return map_(and_then(direc, char("V"), direc), lambda v: WindVaryExpr(v[0], v[-1]))


def visibility_expr() -> Parser:
    digit = digit_expr()
    return map_(n_or_more(4, digit), lambda r: VisibilityExpr(join(r)))


def runway_expr() -> Parser:
    r = char("R")
    digit = digit_expr()
    lorr = or_(char("L"), char("R"))
    sep = char("/")
    uordorn = any_(char("U"), char("D"), char("N"))

    runway = and_then(r, map_(and_then(n_or_more(2, digit), zero_or_more(lorr)), join))
    vis = map_(and_then(n_or_more(4, digit), zero_or_more(uordorn)), join)
    return map_(and_then(runway, sep, vis), tuple)


def cloud_expr() -> Parser:
    desc = [
        identifier(i) for i in ["SKC", "FEW", "SCT", "BKN", "OVC", "NSC", "NCD", "CLR"]
    ]
    digit = digit_expr()
    cb = identifier("CB")

    height = map_(n_or_more(3, digit), join)
    return map_(
        and_then(any_(*desc), zero_or_more(height), zero_or_more(cb)),
        lambda v: CloudExpr(*v),
    )


def weather_expr() -> Parser:
    rain_snow = any_(
        *[identifier(i) for i in ["DZ", "RA", "SN", "SG", "IC", "PL", "GR", "GS", "UP"]]
    )
    fog = any_(
        *[identifier(i) for i in ["BR", "FG", "FU", "VA", "SA", "HZ", "PY", "DU"]]
    )
    defi = any_(
        *[
            identifier(i)
            for i in ["MI", "BC", "PR", "TS", "BL", "SH", "DR", "FZ", "-", "+"]
        ]
    )
    return map_(and_then(zero_or_more(defi), any_(rain_snow, fog)), tuple)


def tmpr_expr() -> Parser:
    digit = digit_expr()
    m = char("M")
    sep = char("/")
    single_tmpr = map_(and_then(zero_or_more(m), n_or_more(2, digit)), join)
    return map_(
        and_then(single_tmpr, sep, single_tmpr), lambda v: TemperatureExpr(v[0], v[-1])
    )


def pres_expr() -> Parser:
    q = char("Q")
    a = char("A")
    digit = digit_expr()

    qora = or_(q, a)
    return map_(
        and_then(qora, map_(n_or_more(4, digit), join)), lambda v: PressureExpr(*v)
    )


def trend_expr() -> Parser:
    digit = digit_expr()
    nosig = identifier("NOSIG")
    becmg = identifier("BECMG")
    tempo = identifier("TEMPO")
    prob = and_then(identifier("PROB"), n_or_more(2, digit))
    fm_tl_at = and_then(
        any_(identifier("FM"), identifier("TL"), identifier("AT")), n_or_more(4, digit)
    )
    return map_(any_(nosig, becmg, tempo, prob, fm_tl_at), join)


def full_expr() -> Parser:
    return one_or_more(
        and_then(
            any_(
                metar_expr(),
                identifier("CAVOK"),  # Put this before ICAO Parser
                trend_expr(),
                icao_expr(),
                time_expr(),
                wind_expr(),
                wind_vary_expr(),
                visibility_expr(),
                runway_expr(),
                weather_expr(),
                cloud_expr(),
                tmpr_expr(),
                pres_expr(),
            ),
            or_(char(" "), char("=")),
        )
    )


def test_zero_or_more():
    p = zero_or_more(identifier("ha"))
    print(p(Stream("hahaha", 0)))
    print(p(Stream("wocao", 0)))


def test_one_or_more():
    p = one_or_more(identifier("ha"))
    print(p(Stream("hahaha", 0)))
    print(p(Stream("wocao", 0)))


def test_failed_llk():
    """Because of the limit of LL(k), this case will failed."""
    p = and_then(or_(icao_expr(), identifier("CAVOK")), char(" "))
    assert p(Stream("CAVOK ", 0)) == None
    p = and_then(or_(identifier("CAVOK"), icao_expr()), char(" "))
    assert p(Stream("CAVOK ", 0)) != None


if __name__ == "__main__":
    metar_report = "METAR/SPECI ZUCK 221630Z 24002MPS 0600 R02L/1000U FZFG SCT010 M02/M02 Q1018 BECMG TL1700 0800 BECMG AT1800 3000 BR="
    metar_parser = full_expr()
    print(metar_parser(Stream(metar_report, 0)))
    with open("ZSPD.txt", "r") as f:
        for line in f.readlines():
            line = line.replace("\n", "")
            s = Stream(line, 0)
            o = metar_parser(s)
            if not o:
                continue
            if o.stream.idx == len(line):
                print(o.value)
            else:
                print(o.stream.idx, len(line), o)
