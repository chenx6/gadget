"""
Show xref tree like dnSpy
Refs:
    https://leanpub.com/IDAPython-Book
    https://doc.qt.io/qt-6/qtreeview.html
    https://blog.xorhex.com/blog/ida-plugin-contextmenu/
"""
from collections import defaultdict

from idc import get_func_name, get_name
from ida_funcs import get_func
from idautils import XrefsTo
from idaapi import (
    PluginForm,
    action_handler_t,
    UI_Hooks,
    get_widget_type,
    BWN_DISASM,
    BWN_PSEUDOCODE,
    action_desc_t,
    attach_dynamic_action_to_popup,
    SETMENU_INS,
    get_screen_ea,
)
from ida_kernwin import jumpto
from PyQt5.QtWidgets import QVBoxLayout, QTreeWidget, QTreeWidgetItem


def generate_info(addr: int):
    name = get_func_name(addr)
    if not name:
        # addr isn't points to a function
        name = get_name(addr)
        if not name:
            name = hex(addr)
        addr = addr
    else:
        fnc = get_func(addr)
        addr = fnc.start_ea
    return name, addr


class XrefForm(PluginForm):
    def __init__(self, root_addr: int):
        super().__init__()
        self.root_addr = root_addr
        self.xrefed = defaultdict(lambda: False)

    def OnCreate(self, form):
        # Get parent widget
        self.parent = self.FormToPyQtWidget(form)  # IDAPython
        self.populate_form()

    def populate_form(self):
        # Use TreeWidget to show references
        self.tree = QTreeWidget()
        self.tree.setColumnCount(3)
        self.tree.setHeaderLabels(["Function name", "Function address", "Xref address"])
        self.tree.setExpandsOnDoubleClick(False)
        self.tree.clicked.connect(self.click_tree_item)
        self.tree.doubleClicked.connect(self.doubleclk_tree_item)

        # Generate root_addr's info as root item
        name, addr = generate_info(self.root_addr)
        item = QTreeWidgetItem(self.tree)
        item.setText(0, name)
        item.setText(1, hex(addr))
        item.setText(2, hex(addr))

        layout = QVBoxLayout()
        layout.addWidget(self.tree)
        self.parent.setLayout(layout)

    def click_tree_item(self, idx):
        item = self.tree.currentItem()
        addr = item.text(1)
        if self.xrefed[addr]:
            # Skip already generated xref address
            return True
        for xref in XrefsTo(int(addr, 16), 0):
            # Add xref to item's child
            xname, xaddr = generate_info(xref.frm)
            child = QTreeWidgetItem(item)
            child.setText(0, xname)
            child.setText(1, hex(xaddr))
            child.setText(2, hex(xref.frm))
        self.tree.expandItem(item)
        self.xrefed[addr] = True

    def doubleclk_tree_item(self, idx):
        item = self.tree.currentItem()
        addr = item.text(2)
        jumpto(int(addr, 16))


class handler_class(action_handler_t):
    def activate(self, ctx):
        ea = get_screen_ea()
        plg = XrefForm(ea)
        plg.Show(f"Xref to {ea:x}")

    def update(self, ctx):
        pass


class ContextHooks(UI_Hooks):
    def finish_populating_widget_popup(self, form, popup):
        tft = get_widget_type(form)
        if tft == BWN_DISASM:
            action_name = action_desc_t(None, "Xref tree", handler_class())
            attach_dynamic_action_to_popup(
                form,
                popup,
                action_name,
                "Xref tree",
                SETMENU_INS,
            )
        elif tft == BWN_PSEUDOCODE:
            pass


hooks = ContextHooks()
hooks.hook()
