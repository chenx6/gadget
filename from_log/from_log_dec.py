"""
https://www.hex-rays.com/products/decompiler/manual/sdk/hexrays_8hpp_source.shtml
https://blog.gentlecp.com/article/12309.html
"""
import idaapi
import ida_hexrays
import ida_bytes
import idautils
import idc


class LogVisitor(ida_hexrays.ctree_visitor_t):
    """Recover log by visiting ctree"""

    def __init__(
        self, cfunc: ida_hexrays.cfunc_t, log_func: str, name_idx: int
    ) -> None:
        super().__init__(ida_hexrays.CV_FAST)
        self.log_func = log_func
        self.name_idx = name_idx
        self.cfunc = cfunc

    def visit_expr(self, expr: ida_hexrays.cexpr_t) -> int:
        ea = self.cfunc.entry_ea
        # named, skip
        if ea in named:
            return 0
        # only need call expression
        if expr.op != ida_hexrays.cot_call:
            return 0
        # call expression is iog_func
        if idaapi.get_func_name(expr.x.obj_ea) != self.log_func:
            return 0
        # length of call argument < idx
        if len(expr.a) < self.name_idx:
            return 0
        # get argument
        carg = expr.a[self.name_idx]
        if carg.op != ida_hexrays.cot_obj:
            return 0
        arg_str = ida_bytes.get_strlit_contents(carg.obj_ea, -1, 0)
        if not arg_str:
            return 0
        print(hex(ea), arg_str)
        # set name
        idc.set_name(ea, arg_str.decode())
        named.add(ea)
        return 0


func_addr = 0x00015CDC  # XXX: EDIT THIS VALUE
name_idx = 4  # XXX: EDIT THIS VALUE
func_name = idaapi.get_func_name(func_addr)
named = set()
for xref_addr in idautils.CodeRefsTo(func_addr, 0):
    try:
        cfunc = idaapi.decompile(xref_addr)
    except Exception as e:
        print(e)
        continue
    if not cfunc or not cfunc.body:
        continue
    v = LogVisitor(cfunc, func_name, name_idx)
    v.apply_to(cfunc.body, None)
