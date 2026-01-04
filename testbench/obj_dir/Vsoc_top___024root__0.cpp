// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsoc_top.h for the primary calling header

#include "Vsoc_top__pch.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsoc_top___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vsoc_top___024root___eval_triggers__ico(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_triggers__ico\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VicoTriggered
                                      [0U]) | (IData)((IData)(vlSelfRef.__VicoFirstIteration)));
    vlSelfRef.__VicoFirstIteration = 0U;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsoc_top___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
    }
#endif
}

bool Vsoc_top___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___trigger_anySet__ico\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vsoc_top___024root___ico_sequent__TOP__0(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___ico_sequent__TOP__0\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_write 
        = ((IData)(vlSelfRef.rst_n) & ((~ (0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) 
                                       & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata)));
    vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word 
        = vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word_q;
    if ((1U & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__prefetched_high_word)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word = 0U;
    }
    if ((1U & (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch) 
                | (0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__irq_state))) 
               | (~ (IData)(vlSelfRef.rst_n))))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word = 0U;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_done = ((IData)(vlSelfRef.rst_n) 
                                                  & (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer) 
                                                      & ((0U 
                                                          != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state)) 
                                                         & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT____VdfgRegularize_h233421b0_0_0))) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state)) 
                                                        & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst))));
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_read = 
        ((IData)(vlSelfRef.rst_n) & ((~ (0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) 
                                     & ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
                                        | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch) 
                                           | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata)))));
    vlSelfRef.soc_top__DOT__cpu_mem_rdata = 0U;
    if ((0U == (0xffff0000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_rdata = vlSelfRef.soc_top__DOT__rom_rdata;
    } else if ((0x10000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_rdata = vlSelfRef.soc_top__DOT__ram_rdata;
    } else if ((0x20000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_rdata = ((0x20000023U 
                                                  == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                                                  ? (IData)(vlSelfRef.gpio_pin_in)
                                                  : 
                                                 ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)
                                                   ? 
                                                  ((0x20000024U 
                                                    == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                                                    ? (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__ddrb)
                                                    : 
                                                   ((0x20000025U 
                                                     == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                                                     ? (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__portb)
                                                     : 0U))
                                                   : 0U));
    }
    if ((0U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
            = vlSelfRef.soc_top__DOT__cpu_mem_rdata;
    } else if ((1U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize))) {
        if ((2U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)) {
            if ((2U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)) {
                vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
                    = (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                       >> 0x10U);
            }
        } else {
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
                = (0x0000ffffU & vlSelfRef.soc_top__DOT__cpu_mem_rdata);
        }
    } else if ((2U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
            = ((2U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)
                ? ((1U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)
                    ? (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                       >> 0x18U) : (0x000000ffU & (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                                                   >> 0x10U)))
                : ((1U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)
                    ? (0x000000ffU & (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                                      >> 8U)) : (0x000000ffU 
                                                 & vlSelfRef.soc_top__DOT__cpu_mem_rdata)));
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer)
            ? vlSelfRef.soc_top__DOT__cpu_mem_rdata
            : vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q);
}

void Vsoc_top___024root___eval_ico(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_ico\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered[0U])) {
        Vsoc_top___024root___ico_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[1U] = 1U;
    }
}

bool Vsoc_top___024root___eval_phase__ico(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_phase__ico\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VicoExecute;
    // Body
    Vsoc_top___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = Vsoc_top___024root___trigger_anySet__ico(vlSelfRef.__VicoTriggered);
    if (__VicoExecute) {
        Vsoc_top___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsoc_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vsoc_top___024root___eval_triggers__act(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_triggers__act\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((IData)(vlSelfRef.clk) 
                                                     & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0)))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsoc_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
}

bool Vsoc_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

extern const VlUnpacked<VlWide<4>/*127:0*/, 256> Vsoc_top__ConstPool__TABLE_h57ed7a03_0;

void Vsoc_top___024root___nba_sequent__TOP__0(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___nba_sequent__TOP__0\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*7:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*1:0*/ __Vdly__soc_top__DOT__cpu__DOT__mem_state;
    __Vdly__soc_top__DOT__cpu__DOT__mem_state = 0;
    CData/*4:0*/ __Vdly__soc_top__DOT__cpu__DOT__reg_sh;
    __Vdly__soc_top__DOT__cpu__DOT__reg_sh = 0;
    IData/*31:0*/ __Vdly__soc_top__DOT__cpu__DOT__reg_out;
    __Vdly__soc_top__DOT__cpu__DOT__reg_out = 0;
    QData/*63:0*/ __Vdly__soc_top__DOT__cpu__DOT__count_cycle;
    __Vdly__soc_top__DOT__cpu__DOT__count_cycle = 0;
    CData/*0:0*/ __Vdly__soc_top__DOT__cpu__DOT__decoder_trigger;
    __Vdly__soc_top__DOT__cpu__DOT__decoder_trigger = 0;
    CData/*0:0*/ __Vdly__soc_top__DOT__cpu__DOT__decoder_pseudo_trigger;
    __Vdly__soc_top__DOT__cpu__DOT__decoder_pseudo_trigger = 0;
    CData/*0:0*/ __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst;
    __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = 0;
    CData/*1:0*/ __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize;
    __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = 0;
    CData/*0:0*/ __Vdly__soc_top__DOT__cpu__DOT__latched_is_lu;
    __Vdly__soc_top__DOT__cpu__DOT__latched_is_lu = 0;
    CData/*0:0*/ __Vdly__soc_top__DOT__cpu__DOT__latched_is_lh;
    __Vdly__soc_top__DOT__cpu__DOT__latched_is_lh = 0;
    CData/*0:0*/ __Vdly__soc_top__DOT__cpu__DOT__latched_is_lb;
    __Vdly__soc_top__DOT__cpu__DOT__latched_is_lb = 0;
    IData/*31:0*/ __Vdly__soc_top__DOT__cpu__DOT__reg_pc;
    __Vdly__soc_top__DOT__cpu__DOT__reg_pc = 0;
    QData/*63:0*/ __Vdly__soc_top__DOT__cpu__DOT__count_instr;
    __Vdly__soc_top__DOT__cpu__DOT__count_instr = 0;
    CData/*0:0*/ __Vdly__soc_top__DOT__cpu__DOT__mem_do_prefetch;
    __Vdly__soc_top__DOT__cpu__DOT__mem_do_prefetch = 0;
    CData/*7:0*/ __Vdly__soc_top__DOT__cpu__DOT__cpu_state;
    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0;
    IData/*31:0*/ __Vdly__soc_top__DOT__cpu__DOT__reg_op1;
    __Vdly__soc_top__DOT__cpu__DOT__reg_op1 = 0;
    IData/*31:0*/ __VdlyVal__soc_top__DOT__cpu__DOT__cpuregs__v0;
    __VdlyVal__soc_top__DOT__cpu__DOT__cpuregs__v0 = 0;
    CData/*4:0*/ __VdlyDim0__soc_top__DOT__cpu__DOT__cpuregs__v0;
    __VdlyDim0__soc_top__DOT__cpu__DOT__cpuregs__v0 = 0;
    CData/*0:0*/ __VdlySet__soc_top__DOT__cpu__DOT__cpuregs__v0;
    __VdlySet__soc_top__DOT__cpu__DOT__cpuregs__v0 = 0;
    CData/*7:0*/ __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v0;
    __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v0 = 0;
    SData/*11:0*/ __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v0;
    __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__soc_top__DOT__data_ram__DOT__mem__v0;
    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v0 = 0;
    CData/*7:0*/ __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v1;
    __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v1 = 0;
    SData/*11:0*/ __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v1;
    __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v1 = 0;
    CData/*0:0*/ __VdlySet__soc_top__DOT__data_ram__DOT__mem__v1;
    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v1 = 0;
    CData/*7:0*/ __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v2;
    __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v2 = 0;
    SData/*11:0*/ __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v2;
    __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v2 = 0;
    CData/*0:0*/ __VdlySet__soc_top__DOT__data_ram__DOT__mem__v2;
    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v2 = 0;
    CData/*7:0*/ __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v3;
    __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v3 = 0;
    SData/*11:0*/ __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v3;
    __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v3 = 0;
    CData/*0:0*/ __VdlySet__soc_top__DOT__data_ram__DOT__mem__v3;
    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v3 = 0;
    // Body
    __Vdly__soc_top__DOT__cpu__DOT__reg_sh = vlSelfRef.soc_top__DOT__cpu__DOT__reg_sh;
    __Vdly__soc_top__DOT__cpu__DOT__count_cycle = vlSelfRef.soc_top__DOT__cpu__DOT__count_cycle;
    __Vdly__soc_top__DOT__cpu__DOT__decoder_pseudo_trigger 
        = vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger;
    __Vdly__soc_top__DOT__cpu__DOT__latched_is_lu = vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lu;
    __Vdly__soc_top__DOT__cpu__DOT__latched_is_lh = vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lh;
    __Vdly__soc_top__DOT__cpu__DOT__latched_is_lb = vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lb;
    __Vdly__soc_top__DOT__cpu__DOT__count_instr = vlSelfRef.soc_top__DOT__cpu__DOT__count_instr;
    __Vdly__soc_top__DOT__cpu__DOT__decoder_trigger 
        = vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger;
    __Vdly__soc_top__DOT__cpu__DOT__mem_do_prefetch 
        = vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch;
    __Vdly__soc_top__DOT__cpu__DOT__reg_pc = vlSelfRef.soc_top__DOT__cpu__DOT__reg_pc;
    __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize;
    __Vdly__soc_top__DOT__cpu__DOT__reg_out = vlSelfRef.soc_top__DOT__cpu__DOT__reg_out;
    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state;
    __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst;
    __Vdly__soc_top__DOT__cpu__DOT__reg_op1 = vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1;
    __VdlySet__soc_top__DOT__cpu__DOT__cpuregs__v0 = 0U;
    __Vdly__soc_top__DOT__cpu__DOT__mem_state = vlSelfRef.soc_top__DOT__cpu__DOT__mem_state;
    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v0 = 0U;
    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v1 = 0U;
    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v2 = 0U;
    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v3 = 0U;
    if ((((IData)(vlSelfRef.rst_n) & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_write)) 
         & (0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_rd)))) {
        __VdlyVal__soc_top__DOT__cpu__DOT__cpuregs__v0 
            = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_wrdata;
        __VdlyDim0__soc_top__DOT__cpu__DOT__cpuregs__v0 
            = vlSelfRef.soc_top__DOT__cpu__DOT__latched_rd;
        __VdlySet__soc_top__DOT__cpu__DOT__cpuregs__v0 = 1U;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__trace_valid = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_wait_2 = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__trace_data = 0ULL;
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_wait = 0U;
    if (vlSelfRef.soc_top__DOT__cpu__DOT__launch_next_insn) {
        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_addr 
            = vlSelfRef.soc_top__DOT__cpu__DOT__next_pc;
        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs2val = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val_valid = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs2val_valid = 0U;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__q_ascii_instr 
        = vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_instr;
    vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_imm = vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_imm;
    vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rs1 = vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs1;
    vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rs2 = vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs2;
    vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rd = vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rd;
    vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_opcode 
        = vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_opcode;
    vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word_q 
        = vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word;
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0_q = vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0;
    vlSelfRef.soc_top__DOT__cpu__DOT__next_irq_pending = 0U;
    if ((1U & (~ (IData)(vlSelfRef.rst_n)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__eoi = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__pcpi_valid = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__irq_mask = 0xffffffffU;
        vlSelfRef.soc_top__DOT__cpu__DOT__pcpi_timeout = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__latched_trace = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__next_irq_pending = 0U;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__irq_pending = vlSelfRef.soc_top__DOT__cpu__DOT__next_irq_pending;
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_next = vlSelfRef.soc_top__DOT__cpu__DOT__launch_next_insn;
    vlSelfRef.soc_top__DOT__rom_rdata_valid = ((IData)(vlSelfRef.rst_n) 
                                               && (IData)(vlSelfRef.soc_top__DOT__program_rom__DOT__ce));
    vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger_q 
        = vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger;
    vlSelfRef.soc_top__DOT__ram_rdata_valid = ((IData)(vlSelfRef.rst_n) 
                                               && (IData)(vlSelfRef.soc_top__DOT__data_ram__DOT__ce));
    vlSelfRef.soc_top__DOT__gpio_mem_ready = ((IData)(vlSelfRef.rst_n) 
                                              && ((IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_valid) 
                                                  & (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__gpio_sel)));
    if (vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger_q) {
        vlSelfRef.soc_top__DOT__cpu__DOT__cached_ascii_instr 
            = vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr;
        vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_opcode 
            = ((3U == (3U & vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode))
                ? vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode
                : (0x0000ffffU & vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode));
        vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rd 
            = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rd;
        vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rs1 
            = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1;
        vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rs2 
            = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2;
        vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_imm 
            = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm;
    }
    if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.soc_top__DOT__cpu_trap)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_valid_insn = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__prefetched_high_word = 0U;
    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__launch_next_insn) {
        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_valid_insn = 1U;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word) {
        vlSelfRef.soc_top__DOT__cpu__DOT__prefetched_high_word = 0U;
    }
    if (__VdlySet__soc_top__DOT__cpu__DOT__cpuregs__v0) {
        vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[__VdlyDim0__soc_top__DOT__cpu__DOT__cpuregs__v0] 
            = __VdlyVal__soc_top__DOT__cpu__DOT__cpuregs__v0;
    }
    if (vlSelfRef.rst_n) {
        if (vlSelfRef.soc_top__DOT__data_ram__DOT__ce) {
            if (vlSelfRef.soc_top__DOT__data_ram__DOT__we) {
                if ((1U & (IData)(vlSelfRef.soc_top__DOT__cpu_mem_wstrb))) {
                    __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v0 
                        = (0x000000ffU & vlSelfRef.soc_top__DOT__cpu_mem_wdata);
                    __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v0 
                        = vlSelfRef.soc_top__DOT__data_ram__DOT__addr;
                    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v0 = 1U;
                }
                if ((2U & (IData)(vlSelfRef.soc_top__DOT__cpu_mem_wstrb))) {
                    __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v1 
                        = (0x000000ffU & (vlSelfRef.soc_top__DOT__cpu_mem_wdata 
                                          >> 8U));
                    __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v1 
                        = vlSelfRef.soc_top__DOT__data_ram__DOT__addr;
                    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v1 = 1U;
                }
                if ((4U & (IData)(vlSelfRef.soc_top__DOT__cpu_mem_wstrb))) {
                    __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v2 
                        = (0x000000ffU & (vlSelfRef.soc_top__DOT__cpu_mem_wdata 
                                          >> 0x10U));
                    __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v2 
                        = vlSelfRef.soc_top__DOT__data_ram__DOT__addr;
                    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v2 = 1U;
                }
                if ((8U & (IData)(vlSelfRef.soc_top__DOT__cpu_mem_wstrb))) {
                    __VdlyVal__soc_top__DOT__data_ram__DOT__mem__v3 
                        = (vlSelfRef.soc_top__DOT__cpu_mem_wdata 
                           >> 0x18U);
                    __VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v3 
                        = vlSelfRef.soc_top__DOT__data_ram__DOT__addr;
                    __VdlySet__soc_top__DOT__data_ram__DOT__mem__v3 = 1U;
                }
                vlSelfRef.soc_top__DOT__ram_rdata = 
                    vlSelfRef.soc_top__DOT__data_ram__DOT__mem
                    [vlSelfRef.soc_top__DOT__data_ram__DOT__addr];
            } else {
                vlSelfRef.soc_top__DOT__ram_rdata = 
                    vlSelfRef.soc_top__DOT__data_ram__DOT__mem
                    [vlSelfRef.soc_top__DOT__data_ram__DOT__addr];
            }
        }
    } else {
        vlSelfRef.soc_top__DOT__ram_rdata = 0U;
    }
    if (__VdlySet__soc_top__DOT__data_ram__DOT__mem__v0) {
        vlSelfRef.soc_top__DOT__data_ram__DOT__mem[__VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v0] 
            = ((0xffffff00U & vlSelfRef.soc_top__DOT__data_ram__DOT__mem
                [__VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v0]) 
               | (IData)(__VdlyVal__soc_top__DOT__data_ram__DOT__mem__v0));
    }
    if (__VdlySet__soc_top__DOT__data_ram__DOT__mem__v1) {
        vlSelfRef.soc_top__DOT__data_ram__DOT__mem[__VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v1] 
            = ((0xffff00ffU & vlSelfRef.soc_top__DOT__data_ram__DOT__mem
                [__VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v1]) 
               | ((IData)(__VdlyVal__soc_top__DOT__data_ram__DOT__mem__v1) 
                  << 8U));
    }
    if (__VdlySet__soc_top__DOT__data_ram__DOT__mem__v2) {
        vlSelfRef.soc_top__DOT__data_ram__DOT__mem[__VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v2] 
            = ((0xff00ffffU & vlSelfRef.soc_top__DOT__data_ram__DOT__mem
                [__VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v2]) 
               | ((IData)(__VdlyVal__soc_top__DOT__data_ram__DOT__mem__v2) 
                  << 0x00000010U));
    }
    if (__VdlySet__soc_top__DOT__data_ram__DOT__mem__v3) {
        vlSelfRef.soc_top__DOT__data_ram__DOT__mem[__VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v3] 
            = ((0x00ffffffU & vlSelfRef.soc_top__DOT__data_ram__DOT__mem
                [__VdlyDim0__soc_top__DOT__data_ram__DOT__mem__v3]) 
               | ((IData)(__VdlyVal__soc_top__DOT__data_ram__DOT__mem__v3) 
                  << 0x00000018U));
    }
    if ((1U & (~ ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.soc_top__DOT__cpu_trap))))) {
        if (vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_write) {
            vlSelfRef.soc_top__DOT__cpu_mem_wdata = vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wdata;
        }
    }
    if (vlSelfRef.rst_n) {
        if ((((IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_valid) 
              & (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__gpio_sel)) 
             & (0U != (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wstrb)))) {
            if ((0x20000024U == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)) {
                if ((1U & (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wstrb))) {
                    vlSelfRef.soc_top__DOT__gpio_inst__DOT__ddrb 
                        = (0x000000ffU & vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wdata);
                }
            }
            if ((0x20000024U != vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)) {
                if ((0x20000025U == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)) {
                    if ((1U & (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wstrb))) {
                        vlSelfRef.soc_top__DOT__gpio_inst__DOT__portb 
                            = (0x000000ffU & vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wdata);
                    }
                }
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__last_mem_valid)))) {
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_firstword_reg = 0U;
        }
        if (vlSelfRef.soc_top__DOT__program_rom__DOT__ce) {
            vlSelfRef.soc_top__DOT__rom_rdata = vlSelfRef.soc_top__DOT__program_rom__DOT__mem
                [((IData)(vlSelfRef.soc_top__DOT__program_rom__DOT__ce)
                   ? (0x00003fffU & (vlSelfRef.soc_top__DOT__cpu_mem_addr 
                                     >> 2U)) : 0U)];
        }
    } else {
        vlSelfRef.soc_top__DOT__gpio_inst__DOT__ddrb = 0U;
        vlSelfRef.soc_top__DOT__gpio_inst__DOT__portb = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_firstword_reg = 0U;
        vlSelfRef.soc_top__DOT__rom_rdata = 0x00000013U;
    }
    if ((1U & (~ ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.soc_top__DOT__cpu_trap))))) {
        if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_read) 
             | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_write))) {
            vlSelfRef.soc_top__DOT__cpu_mem_addr = 
                (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch) 
                  | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst))
                  ? (0xfffffffcU & vlSelfRef.soc_top__DOT__cpu__DOT__next_pc)
                  : (0xfffffffcU & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1));
        }
    }
    vlSelfRef.gpio_pin_dir = vlSelfRef.soc_top__DOT__gpio_inst__DOT__ddrb;
    vlSelfRef.gpio_pin_out = vlSelfRef.soc_top__DOT__gpio_inst__DOT__portb;
    vlSelfRef.soc_top__DOT__cpu__DOT__last_mem_valid 
        = ((IData)(vlSelfRef.rst_n) && ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid) 
                                        & (~ (IData)(vlSelfRef.soc_top__DOT__cpu_mem_ready))));
    if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.soc_top__DOT__cpu_trap)))) {
        if ((1U & (~ (IData)(vlSelfRef.rst_n)))) {
            __Vdly__soc_top__DOT__cpu__DOT__mem_state = 0U;
        }
        if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.soc_top__DOT__cpu_mem_ready)))) {
            vlSelfRef.soc_top__DOT__cpu_mem_valid = 0U;
        }
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_secondword = 0U;
    } else {
        if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_read) 
             | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_write))) {
            vlSelfRef.soc_top__DOT__cpu_mem_wstrb = 
                ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb) 
                 & (- (IData)((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_write))));
        }
        if ((0U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) {
            if ((((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch) 
                  | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst)) 
                 | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata))) {
                vlSelfRef.soc_top__DOT__cpu_mem_valid = 1U;
                vlSelfRef.soc_top__DOT__cpu_mem_instr 
                    = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch) 
                       | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst));
                vlSelfRef.soc_top__DOT__cpu_mem_wstrb = 0U;
                __Vdly__soc_top__DOT__cpu__DOT__mem_state = 1U;
            }
            if (vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata) {
                vlSelfRef.soc_top__DOT__cpu_mem_valid = 1U;
                vlSelfRef.soc_top__DOT__cpu_mem_instr = 0U;
                __Vdly__soc_top__DOT__cpu__DOT__mem_state = 2U;
            }
        } else if ((1U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) {
            if (vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer) {
                vlSelfRef.soc_top__DOT__cpu_mem_valid = 0U;
                vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_secondword = 0U;
                __Vdly__soc_top__DOT__cpu__DOT__mem_state 
                    = (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
                        | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata))
                        ? 0U : 3U);
            }
        } else if ((2U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) {
            if (vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer) {
                vlSelfRef.soc_top__DOT__cpu_mem_valid = 0U;
                __Vdly__soc_top__DOT__cpu__DOT__mem_state = 0U;
            }
        } else if ((3U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) {
            if (vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) {
                __Vdly__soc_top__DOT__cpu__DOT__mem_state = 0U;
            }
        }
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_opcode 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_opcode;
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs1 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rs1;
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_state = __Vdly__soc_top__DOT__cpu__DOT__mem_state;
    vlSelfRef.soc_top__DOT__cpu_mem_ready = 0U;
    vlSelfRef.soc_top__DOT__program_rom__DOT__ce = 
        ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid) 
         & (0U == (0xffff0000U & vlSelfRef.soc_top__DOT__cpu_mem_addr)));
    vlSelfRef.soc_top__DOT__data_ram__DOT__ce = ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid) 
                                                 & (0x10000000U 
                                                    == 
                                                    (0xfffff000U 
                                                     & vlSelfRef.soc_top__DOT__cpu_mem_addr)));
    vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_valid 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid) 
           & (0x20000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr)));
    vlSelfRef.soc_top__DOT__cpu_trap = 0U;
    __Vdly__soc_top__DOT__cpu__DOT__reg_sh = 0U;
    __Vdly__soc_top__DOT__cpu__DOT__reg_out = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_rinst = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_rdata = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_wdata = 0U;
    __Vdly__soc_top__DOT__cpu__DOT__decoder_trigger 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
           & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done));
    vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger_q 
        = vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger;
    __Vdly__soc_top__DOT__cpu__DOT__decoder_pseudo_trigger = 0U;
    if (vlSelfRef.rst_n) {
        __Vdly__soc_top__DOT__cpu__DOT__count_cycle 
            = (1ULL + vlSelfRef.soc_top__DOT__cpu__DOT__count_cycle);
        if (((((((((0x80U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state)) 
                   | (0x40U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) 
                  | (0x20U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) 
                 | (0x10U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) 
                | (8U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) 
               | (4U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) 
              | (2U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) 
             | (1U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state)))) {
            if ((0x80U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) {
                vlSelfRef.soc_top__DOT__cpu_trap = 1U;
            } else if ((0x40U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) {
                __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst 
                    = (1U & ((~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger)) 
                             & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__do_waitirq))));
                __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = 0U;
                __Vdly__soc_top__DOT__cpu__DOT__latched_is_lu = 0U;
                __Vdly__soc_top__DOT__cpu__DOT__latched_is_lh = 0U;
                __Vdly__soc_top__DOT__cpu__DOT__latched_is_lb = 0U;
                vlSelfRef.soc_top__DOT__cpu__DOT__latched_rd 
                    = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rd;
                vlSelfRef.soc_top__DOT__cpu__DOT__latched_compr 
                    = vlSelfRef.soc_top__DOT__cpu__DOT__compressed_instr;
                vlSelfRef.soc_top__DOT__cpu__DOT__current_pc 
                    = vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc;
                if (vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch) {
                    vlSelfRef.soc_top__DOT__cpu__DOT__current_pc 
                        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_store)
                            ? (0xfffffffeU & ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_stalu)
                                               ? vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_q
                                               : vlSelfRef.soc_top__DOT__cpu__DOT__reg_out))
                            : vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc);
                }
                __Vdly__soc_top__DOT__cpu__DOT__reg_pc 
                    = vlSelfRef.soc_top__DOT__cpu__DOT__current_pc;
                vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc 
                    = vlSelfRef.soc_top__DOT__cpu__DOT__current_pc;
                vlSelfRef.soc_top__DOT__cpu__DOT__latched_store = 0U;
                vlSelfRef.soc_top__DOT__cpu__DOT__latched_stalu = 0U;
                vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch = 0U;
                if (vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger) {
                    __Vdly__soc_top__DOT__cpu__DOT__count_instr 
                        = (1ULL + vlSelfRef.soc_top__DOT__cpu__DOT__count_instr);
                    vlSelfRef.soc_top__DOT__cpu__DOT__irq_delay 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__irq_active;
                    vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc 
                        = (vlSelfRef.soc_top__DOT__cpu__DOT__current_pc 
                           + ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__compressed_instr)
                               ? 2U : 4U));
                    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_jal) {
                        __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = 1U;
                        vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc 
                            = (vlSelfRef.soc_top__DOT__cpu__DOT__current_pc 
                               + vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j);
                        vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch = 1U;
                    } else {
                        __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = 0U;
                        __Vdly__soc_top__DOT__cpu__DOT__mem_do_prefetch 
                            = (1U & ((~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr)) 
                                     & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__compressed_instr))));
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x20U;
                    }
                }
            } else if ((0x20U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) {
                __Vdly__soc_top__DOT__cpu__DOT__reg_op1 = 0U;
                vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 = 0U;
                if ((((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_trap) 
                      | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_rdcycle_rdcycleh_rdinstr_rdinstrh)) 
                     | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_lui_auipc_jal))) {
                    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_trap) {
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x80U;
                    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_rdcycle_rdcycleh_rdinstr_rdinstrh) {
                        if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycle) {
                            __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                                = (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__count_cycle);
                        } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycleh) {
                            __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                                = (IData)((vlSelfRef.soc_top__DOT__cpu__DOT__count_cycle 
                                           >> 0x20U));
                        } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstr) {
                            __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                                = (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__count_instr);
                        } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstrh) {
                            __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                                = (IData)((vlSelfRef.soc_top__DOT__cpu__DOT__count_instr 
                                           >> 0x20U));
                        }
                        vlSelfRef.soc_top__DOT__cpu__DOT__latched_store = 1U;
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x40U;
                    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_lui_auipc_jal) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lui)
                                ? 0U : vlSelfRef.soc_top__DOT__cpu__DOT__reg_pc);
                        vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm;
                        __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch;
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 8U;
                    } else {
                        vlSelfRef.soc_top__DOT__cpu__DOT__latched_store = 1U;
                        __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__timer;
                        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                        vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val_valid = 1U;
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x40U;
                        vlSelfRef.soc_top__DOT__cpu__DOT__timer 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    }
                } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu) 
                            & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_trap)))) {
                    __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val_valid = 1U;
                    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 1U;
                    __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = 1U;
                } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_slli_srli_srai) {
                    __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val_valid = 1U;
                    __Vdly__soc_top__DOT__cpu__DOT__reg_sh 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2;
                    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 4U;
                } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_jalr_addi_slti_sltiu_xori_ori_andi) {
                    __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val_valid = 1U;
                    vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm;
                    __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch;
                    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 8U;
                } else {
                    __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val_valid = 1U;
                    __Vdly__soc_top__DOT__cpu__DOT__reg_sh 
                        = (0x0000001fU & vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2);
                    vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs2val 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2;
                    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs2val_valid = 1U;
                    if (vlSelfRef.soc_top__DOT__cpu__DOT__is_sb_sh_sw) {
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 2U;
                        __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = 1U;
                    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_sll_srl_sra) {
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 4U;
                    } else {
                        __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch;
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 8U;
                    }
                }
            } else if ((0x10U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) {
                __Vdly__soc_top__DOT__cpu__DOT__reg_sh 
                    = (0x0000001fU & vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2);
                vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                    = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2;
                vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs2val 
                    = vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2;
                vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs2val_valid = 1U;
                if (vlSelfRef.soc_top__DOT__cpu__DOT__is_sb_sh_sw) {
                    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 2U;
                    __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = 1U;
                } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_sll_srl_sra) {
                    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 4U;
                } else {
                    __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch;
                    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 8U;
                }
            } else if ((8U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) {
                __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                    = (vlSelfRef.soc_top__DOT__cpu__DOT__reg_pc 
                       + vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm);
                if (vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu) {
                    vlSelfRef.soc_top__DOT__cpu__DOT__latched_rd = 0U;
                    vlSelfRef.soc_top__DOT__cpu__DOT__latched_store 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0;
                    vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0;
                    if (vlSelfRef.soc_top__DOT__cpu__DOT__mem_done) {
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x40U;
                    }
                    if (vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0) {
                        vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_rinst = 1U;
                        __Vdly__soc_top__DOT__cpu__DOT__decoder_trigger = 0U;
                    }
                } else {
                    vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr;
                    vlSelfRef.soc_top__DOT__cpu__DOT__latched_store = 1U;
                    vlSelfRef.soc_top__DOT__cpu__DOT__latched_stalu = 1U;
                    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x40U;
                }
            } else if ((4U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) {
                vlSelfRef.soc_top__DOT__cpu__DOT__latched_store = 1U;
                if ((0U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__reg_sh))) {
                    __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1;
                    __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst 
                        = vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch;
                    __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x40U;
                } else if ((4U <= (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__reg_sh))) {
                    if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slli) 
                         | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sll))) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = VL_SHIFTL_III(32,32,32, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1, 4U);
                    } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srli) 
                                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srl))) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = VL_SHIFTR_III(32,32,32, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1, 4U);
                    } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srai) 
                                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sra))) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = VL_SHIFTRS_III(32,32,32, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1, 4U);
                    }
                    __Vdly__soc_top__DOT__cpu__DOT__reg_sh 
                        = (0x0000001fU & ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__reg_sh) 
                                          - (IData)(4U)));
                } else {
                    if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slli) 
                         | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sll))) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = VL_SHIFTL_III(32,32,32, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1, 1U);
                    } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srli) 
                                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srl))) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = VL_SHIFTR_III(32,32,32, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1, 1U);
                    } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srai) 
                                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sra))) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = VL_SHIFTRS_III(32,32,32, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1, 1U);
                    }
                    __Vdly__soc_top__DOT__cpu__DOT__reg_sh 
                        = (0x0000001fU & ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__reg_sh) 
                                          - (IData)(1U)));
                }
            } else if ((2U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) {
                if ((1U & ((~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch)) 
                           | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done)))) {
                    if ((1U & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata)))) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                               + vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm);
                        vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_wdata = 1U;
                        if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sb) {
                            __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = 2U;
                        } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sh) {
                            __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = 1U;
                        } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sw) {
                            __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = 0U;
                        }
                    }
                    if (((~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch)) 
                         & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done))) {
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x40U;
                        __Vdly__soc_top__DOT__cpu__DOT__decoder_trigger = 1U;
                        __Vdly__soc_top__DOT__cpu__DOT__decoder_pseudo_trigger = 1U;
                    }
                }
            } else {
                vlSelfRef.soc_top__DOT__cpu__DOT__latched_store = 1U;
                if ((1U & ((~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch)) 
                           | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done)))) {
                    if (((~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch)) 
                         & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done))) {
                        if (vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lu) {
                            __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                                = vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word;
                        } else if (vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lh) {
                            __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                                = VL_EXTENDS_II(32,16, 
                                                (0x0000ffffU 
                                                 & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word));
                        } else if (vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lb) {
                            __Vdly__soc_top__DOT__cpu__DOT__reg_out 
                                = VL_EXTENDS_II(32,8, 
                                                (0x000000ffU 
                                                 & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word));
                        }
                        __Vdly__soc_top__DOT__cpu__DOT__decoder_trigger = 1U;
                        __Vdly__soc_top__DOT__cpu__DOT__decoder_pseudo_trigger = 1U;
                        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x40U;
                    }
                    if ((1U & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata)))) {
                        __Vdly__soc_top__DOT__cpu__DOT__reg_op1 
                            = (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                               + vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm);
                        vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_rdata = 1U;
                        if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lb) 
                             | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lbu))) {
                            __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = 2U;
                        } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lh) 
                                    | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lhu))) {
                            __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = 1U;
                        } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_lw) {
                            __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize = 0U;
                        }
                        __Vdly__soc_top__DOT__cpu__DOT__latched_is_lu 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__is_lbu_lhu_lw;
                        __Vdly__soc_top__DOT__cpu__DOT__latched_is_lh 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__instr_lh;
                        __Vdly__soc_top__DOT__cpu__DOT__latched_is_lb 
                            = vlSelfRef.soc_top__DOT__cpu__DOT__instr_lb;
                    }
                }
            }
        }
    } else {
        __Vdly__soc_top__DOT__cpu__DOT__count_cycle = 0ULL;
        __Vdly__soc_top__DOT__cpu__DOT__count_instr = 0ULL;
        vlSelfRef.soc_top__DOT__cpu__DOT__timer = 0U;
        __Vdly__soc_top__DOT__cpu__DOT__reg_pc = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__latched_store = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__latched_stalu = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch = 0U;
        __Vdly__soc_top__DOT__cpu__DOT__latched_is_lu = 0U;
        __Vdly__soc_top__DOT__cpu__DOT__latched_is_lh = 0U;
        __Vdly__soc_top__DOT__cpu__DOT__latched_is_lb = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__irq_delay = 0U;
        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x40U;
    }
    if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger) 
         & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__pcpi_insn = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_ecall_ebreak 
            = (((0x73U == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)) 
                & (~ (0U != (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                             >> 0x15U)))) & (~ (0U 
                                                != 
                                                (0x00001fffU 
                                                 & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                    >> 7U)))));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_getq = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_setq = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_maskirq = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_timer = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_fence 
            = ((0x0fU == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)) 
               & (~ (0U != (7U & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                  >> 0x0cU)))));
    }
    if (((IData)(vlSelfRef.rst_n) & ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata) 
                                     | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata)))) {
        if (((0U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize)) 
             & (0U != (3U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)))) {
            __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x80U;
        }
        if (((1U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize)) 
             & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)) {
            __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x80U;
        }
    }
    if ((((IData)(vlSelfRef.rst_n) & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst)) 
         & (0U != (3U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_pc)))) {
        __Vdly__soc_top__DOT__cpu__DOT__cpu_state = 0x80U;
    }
    if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done)))) {
        __Vdly__soc_top__DOT__cpu__DOT__mem_do_prefetch = 0U;
        __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata = 0U;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_rinst) {
        __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst = 1U;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_rdata) {
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata = 1U;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_wdata) {
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata = 1U;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__current_pc = 0U;
    if (vlSelfRef.soc_top__DOT__data_ram__DOT__ce) {
        vlSelfRef.soc_top__DOT__data_ram__DOT__we = 
            (0U != (IData)(vlSelfRef.soc_top__DOT__cpu_mem_wstrb));
        vlSelfRef.soc_top__DOT__data_ram__DOT__addr 
            = (0x00000fffU & (vlSelfRef.soc_top__DOT__cpu_mem_addr 
                              >> 2U));
    } else {
        vlSelfRef.soc_top__DOT__data_ram__DOT__we = 0U;
        vlSelfRef.soc_top__DOT__data_ram__DOT__addr = 0U;
    }
    if (vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_valid) {
        vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wdata 
            = vlSelfRef.soc_top__DOT__cpu_mem_wdata;
        vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wstrb 
            = vlSelfRef.soc_top__DOT__cpu_mem_wstrb;
        vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr 
            = vlSelfRef.soc_top__DOT__cpu_mem_addr;
    } else {
        vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wdata = 0U;
        vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wstrb = 0U;
        vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr = 0U;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__reg_sh = __Vdly__soc_top__DOT__cpu__DOT__reg_sh;
    vlSelfRef.soc_top__DOT__cpu__DOT__count_cycle = __Vdly__soc_top__DOT__cpu__DOT__count_cycle;
    vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lu 
        = __Vdly__soc_top__DOT__cpu__DOT__latched_is_lu;
    vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lh 
        = __Vdly__soc_top__DOT__cpu__DOT__latched_is_lh;
    vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lb 
        = __Vdly__soc_top__DOT__cpu__DOT__latched_is_lb;
    vlSelfRef.soc_top__DOT__cpu__DOT__count_instr = __Vdly__soc_top__DOT__cpu__DOT__count_instr;
    if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
         & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_waitirq = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1 
            = (0x0000001fU & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
                              >> 0x0fU));
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1 = 
        ((0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1))
          ? vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs
         [vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1]
          : 0U);
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch 
        = __Vdly__soc_top__DOT__cpu__DOT__mem_do_prefetch;
    vlSelfRef.soc_top__DOT__cpu__DOT__reg_pc = __Vdly__soc_top__DOT__cpu__DOT__reg_pc;
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize 
        = __Vdly__soc_top__DOT__cpu__DOT__mem_wordsize;
    vlSelfRef.soc_top__DOT__cpu__DOT__reg_out = __Vdly__soc_top__DOT__cpu__DOT__reg_out;
    vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state = __Vdly__soc_top__DOT__cpu__DOT__cpu_state;
    vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 = __Vdly__soc_top__DOT__cpu__DOT__reg_op1;
    vlSelfRef.__VdfgRegularize_hebeb780c_0_0 = ((0x20000024U 
                                                 == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr) 
                                                | (0x20000025U 
                                                   == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr));
    vlSelfRef.soc_top__DOT__cpu__DOT__do_waitirq = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_write 
        = ((IData)(vlSelfRef.rst_n) & ((~ (0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) 
                                       & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata)));
    __Vtableidx1 = vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state;
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_state[0U] 
        = Vsoc_top__ConstPool__TABLE_h57ed7a03_0[__Vtableidx1][0U];
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_state[1U] 
        = Vsoc_top__ConstPool__TABLE_h57ed7a03_0[__Vtableidx1][1U];
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_state[2U] 
        = Vsoc_top__ConstPool__TABLE_h57ed7a03_0[__Vtableidx1][2U];
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_state[3U] 
        = Vsoc_top__ConstPool__TABLE_h57ed7a03_0[__Vtableidx1][3U];
    vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_write = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__next_pc = (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch) 
                                                  & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_store))
                                                  ? 
                                                 (0xfffffffeU 
                                                  & vlSelfRef.soc_top__DOT__cpu__DOT__reg_out)
                                                  : vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc);
    vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word 
        = vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word_q;
    if ((1U & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__prefetched_high_word)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word = 0U;
    }
    if ((1U & (~ (IData)(vlSelfRef.rst_n)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__irq_state = 0U;
    }
    if ((1U & (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch) 
                | (0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__irq_state))) 
               | (~ (IData)(vlSelfRef.rst_n))))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word = 0U;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_q = vlSelfRef.soc_top__DOT__cpu__DOT__alu_out;
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_eq = (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                                                == vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_lts = VL_LTS_III(32, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_ltu = (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                                                 < vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
    vlSelfRef.soc_top__DOT__cpu__DOT__is_lbu_lhu_lw 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lbu) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lhu) 
              | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lw)));
    vlSelfRef.soc_top__DOT__gpio_inst__DOT__gpio_sel 
        = ((0x20000023U == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr) 
           | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0));
    vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_wrdata = 0U;
    if ((0x40U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state))) {
        if (vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch) {
            vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_write = 1U;
            vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_wrdata 
                = (vlSelfRef.soc_top__DOT__cpu__DOT__reg_pc 
                   + ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_compr)
                       ? 2U : 4U));
        } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_store) 
                    & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch)))) {
            vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_write = 1U;
            vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_wrdata 
                = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_stalu)
                    ? vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_q
                    : vlSelfRef.soc_top__DOT__cpu__DOT__reg_out);
        }
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rd = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rd;
    if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
         & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rd 
            = (0x0000001fU & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
                              >> 7U));
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2 
            = (0x0000001fU & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
                              >> 0x14U));
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2 = 
        ((0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2))
          ? vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs
         [vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2]
          : 0U);
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs2 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rs2;
    if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger) 
         & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycle 
            = ((IData)((0xc0002073U == (0xfffff07fU 
                                        & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))) 
               | (IData)((0xc0102073U == (0xfffff07fU 
                                          & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycleh 
            = ((IData)((0xc8002073U == (0xfffff07fU 
                                        & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))) 
               | (IData)((0xc8102073U == (0xfffff07fU 
                                          & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstr 
            = (IData)((0xc0202073U == (0xfffff07fU 
                                       & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstrh 
            = (IData)((0xc8202073U == (0xfffff07fU 
                                       & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__is_rdcycle_rdcycleh_rdinstr_rdinstrh 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycle) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycleh) 
              | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstr) 
                 | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstrh))));
    vlSelfRef.soc_top__DOT__cpu__DOT__is_lui_auipc_jal 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lui) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_auipc) 
              | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jal)));
    vlSelfRef.soc_top__DOT__cpu__DOT__is_lui_auipc_jal_jalr_addi_add_sub 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lui) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_auipc) 
              | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jal) 
                 | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr) 
                    | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_addi) 
                       | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_add) 
                          | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sub)))))));
    vlSelfRef.soc_top__DOT__cpu__DOT__is_slti_blt_slt 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slti) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_blt) 
              | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slt)));
    vlSelfRef.soc_top__DOT__cpu__DOT__is_sltiu_bltu_sltu 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltiu) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bltu) 
              | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltu)));
    vlSelfRef.soc_top__DOT__cpu__DOT__is_compare = 
        ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu) 
         | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slti) 
            | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slt) 
               | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltiu) 
                  | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltu)))));
    if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger) 
         & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_beq 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu) 
               & (0U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_bne 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu) 
               & (0x00001000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_blt 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu) 
               & (0x00004000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_bge 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu) 
               & (0x00005000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_bltu 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu) 
               & (0x00006000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_bgeu 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu) 
               & (0x00007000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_lb 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu) 
               & (0U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_lh 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu) 
               & (0x00001000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_lw 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu) 
               & (0x00002000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_lbu 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu) 
               & (0x00004000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_lhu 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu) 
               & (0x00005000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sb 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_sb_sh_sw) 
               & (0U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sh 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_sb_sh_sw) 
               & (0x00001000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sw 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_sb_sh_sw) 
               & (0x00002000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_addi 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_slti 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0x00002000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltiu 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0x00003000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_xori 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0x00004000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_ori 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0x00006000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_andi 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0x00007000U == (0x00007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_slli 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0x00001000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_srli 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0x00005000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_srai 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & (0x40005000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__is_slli_srli_srai 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
               & ((IData)((0x00001000U == (0xfe007000U 
                                           & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))) 
                  | ((IData)((0x00005000U == (0xfe007000U 
                                              & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))) 
                     | (IData)((0x40005000U == (0xfe007000U 
                                                & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))))));
        vlSelfRef.soc_top__DOT__cpu__DOT__is_jalr_addi_slti_sltiu_xori_ori_andi 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr) 
               | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm) 
                  & ((0U == (7U & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                   >> 0x0cU))) | ((2U 
                                                   == 
                                                   (7U 
                                                    & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                       >> 0x0cU))) 
                                                  | ((3U 
                                                      == 
                                                      (7U 
                                                       & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                          >> 0x0cU))) 
                                                     | ((4U 
                                                         == 
                                                         (7U 
                                                          & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                             >> 0x0cU))) 
                                                        | ((6U 
                                                            == 
                                                            (7U 
                                                             & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                                >> 0x0cU))) 
                                                           | (7U 
                                                              == 
                                                              (7U 
                                                               & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                                  >> 0x0cU))))))))));
        vlSelfRef.soc_top__DOT__cpu__DOT__is_lui_auipc_jal_jalr_addi_add_sub = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__is_compare = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jal)
                ? vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j
                : (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lui) 
                    | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_auipc))
                    ? VL_SHIFTL_III(32,32,32, (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                               >> 0x0cU), 0x0000000cU)
                    : (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr) 
                        | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu) 
                           | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm)))
                        ? VL_EXTENDS_II(32,12, (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                >> 0x14U))
                        : ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu)
                            ? VL_EXTENDS_II(32,13, 
                                            ((((2U 
                                                & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                   >> 0x0000001eU)) 
                                               | (1U 
                                                  & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                     >> 7U))) 
                                              << 0x0000000bU) 
                                             | ((0x000007e0U 
                                                 & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                    >> 0x00000014U)) 
                                                | (0x0000001eU 
                                                   & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                      >> 7U)))))
                            : ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_sb_sh_sw)
                                ? VL_EXTENDS_II(32,12, 
                                                ((0x00000fe0U 
                                                  & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                     >> 0x00000014U)) 
                                                 | (0x0000001fU 
                                                    & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
                                                       >> 7U))))
                                : 0U)))));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_add 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sub 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x40000000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sll 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x00001000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_slt 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x00002000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltu 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x00003000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_xor 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x00004000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_srl 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x00005000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sra 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x40005000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_or 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x00006000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_and 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & (0x00007000U == (0xfe007000U & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q)));
        vlSelfRef.soc_top__DOT__cpu__DOT__is_sll_srl_sra 
            = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg) 
               & ((IData)((0x00001000U == (0xfe007000U 
                                           & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))) 
                  | ((IData)((0x00005000U == (0xfe007000U 
                                              & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))) 
                     | (IData)((0x40005000U == (0xfe007000U 
                                                & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q))))));
    }
    if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
         & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__compressed_instr = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm 
            = (0x13U == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle));
        vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu 
            = (3U == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle));
        vlSelfRef.soc_top__DOT__cpu__DOT__is_sb_sh_sw 
            = (0x23U == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle));
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j 
            = ((0x000fffffU & vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j) 
               | (0xfff00000U & VL_EXTENDS_II(32,21, 
                                              (0x001ffffeU 
                                               & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
                                                  >> 0x0000000bU)))));
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j 
            = ((0xfffff801U & vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j) 
               | (0x000007feU & (VL_EXTENDS_II(32,21, 
                                               (0x001ffffeU 
                                                & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
                                                   >> 0x0000000bU))) 
                                 >> 9U)));
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j 
            = ((0xfffff7ffU & vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j) 
               | (0x00000800U & (VL_EXTENDS_II(32,21, 
                                               (0x001ffffeU 
                                                & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
                                                   >> 0x0000000bU))) 
                                 << 2U)));
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j 
            = ((0xfff00fffU & vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j) 
               | (0x000ff000U & (VL_EXTENDS_II(32,21, 
                                               (0x001ffffeU 
                                                & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
                                                   >> 0x0000000bU))) 
                                 << 0x0000000bU)));
        vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j 
            = ((0xfffffffeU & vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j) 
               | (1U & VL_EXTENDS_II(1,21, (0x001ffffeU 
                                            & (vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
                                               >> 0x0000000bU)))));
        vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu 
            = (0x63U == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_auipc 
            = (0x17U == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_lui 
            = (0x37U == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_jal 
            = (0x6fU == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle));
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr 
            = (IData)((0x00000067U == (0x0000707fU 
                                       & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle)));
        vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg 
            = (0x33U == (0x0000007fU & vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle));
    }
    if ((1U & (~ (IData)(vlSelfRef.rst_n)))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_fence = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__irq_active = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__is_compare = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_beq = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_bne = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_blt = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_bge = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_bltu = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_bgeu = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_addi = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_slti = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltiu = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_xori = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_ori = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_andi = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_add = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sub = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sll = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_slt = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltu = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_xor = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_srl = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_sra = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_or = 0U;
        vlSelfRef.soc_top__DOT__cpu__DOT__instr_and = 0U;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_imm 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_imm;
    vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger 
        = __Vdly__soc_top__DOT__cpu__DOT__decoder_pseudo_trigger;
    vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger 
        = __Vdly__soc_top__DOT__cpu__DOT__decoder_trigger;
    vlSelfRef.soc_top__DOT__cpu__DOT__launch_next_insn 
        = ((0x40U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state)) 
           & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger));
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0 = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_out = 0U;
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_beq) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0 
            = vlSelfRef.soc_top__DOT__cpu__DOT__alu_eq;
    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_bne) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0 
            = (1U & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__alu_eq)));
    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_bge) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0 
            = (1U & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__alu_lts)));
    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_bgeu) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0 
            = (1U & (~ (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__alu_ltu)));
    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_slti_blt_slt) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0 
            = vlSelfRef.soc_top__DOT__cpu__DOT__alu_lts;
    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_sltiu_bltu_sltu) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0 
            = vlSelfRef.soc_top__DOT__cpu__DOT__alu_ltu;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__is_lui_auipc_jal_jalr_addi_add_sub) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out = 
            ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sub)
              ? (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                 - vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2)
              : (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                 + vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2));
    } else if (vlSelfRef.soc_top__DOT__cpu__DOT__is_compare) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out = vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0;
    } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_xori) 
                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_xor))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out = 
            (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
             ^ vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
    } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_ori) 
                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_or))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out = 
            (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
             | vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
    } else if (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_andi) 
                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_and))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__alu_out = 
            (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
             & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__instr_trap = 
        (1U & (~ ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lui) 
                  | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_auipc) 
                     | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jal) 
                        | (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr) 
                            | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_beq) 
                               | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bne) 
                                  | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_blt) 
                                     | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bge) 
                                        | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bltu) 
                                           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bgeu) 
                                              | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lb) 
                                                 | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lh) 
                                                    | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lw) 
                                                       | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lbu) 
                                                          | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lhu) 
                                                             | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sb) 
                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sh) 
                                                                   | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sw) 
                                                                      | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_addi) 
                                                                         | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slti) 
                                                                            | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltiu) 
                                                                               | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_xori) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_ori) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_andi) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slli) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srli) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srai) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_add) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sub) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sll) 
                                                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slt) 
                                                                                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltu))))))))))))))))))))))))))))) 
                           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_xor) 
                              | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srl) 
                                 | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sra) 
                                    | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_or) 
                                       | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_and) 
                                          | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycle) 
                                             | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycleh) 
                                                | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstr) 
                                                   | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstrh) 
                                                      | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_fence) 
                                                         | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_getq) 
                                                            | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_setq) 
                                                               | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__compressed_instr) 
                                                                  | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_maskirq) 
                                                                     | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_timer) 
                                                                        | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_waitirq))))))))))))))))))))));
    vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0ULL;
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_lui) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000006c7569ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_auipc) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000006175697063ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_jal) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000006a616cULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x000000006a616c72ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_beq) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000626571ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_bne) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000626e65ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_blt) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000626c74ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_bge) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000626765ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_bltu) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000626c7475ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_bgeu) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000062676575ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_lb) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000006c62ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_lh) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000006c68ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_lw) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000006c77ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_lbu) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000006c6275ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_lhu) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000006c6875ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sb) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000007362ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sh) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000007368ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sw) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000007377ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_addi) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000061646469ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_slti) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000736c7469ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltiu) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x000000736c746975ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_xori) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000786f7269ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_ori) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000006f7269ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_andi) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000616e6469ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_slli) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000736c6c69ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_srli) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000073726c69ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_srai) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000073726169ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_add) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000616464ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sub) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000737562ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sll) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000736c6cULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_slt) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000736c74ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltu) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000000736c7475ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_xor) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000786f72ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_srl) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x000000000073726cULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_sra) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000737261ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_or) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000006f72ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_and) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000000616e64ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycle) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0072646379636c65ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycleh) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x72646379636c6568ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstr) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x007264696e737472ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstrh) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x7264696e73747268ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_fence) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000066656e6365ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_getq) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000067657471ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_setq) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000000073657471ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__compressed_instr) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0000726574697271ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_maskirq) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x006d61736b697271ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_waitirq) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x0077616974697271ULL;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__instr_timer) {
        vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr = 0x00000074696d6572ULL;
    }
    if ((0U == (0xffff0000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_ready = vlSelfRef.soc_top__DOT__rom_rdata_valid;
    } else if ((0x10000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_ready = vlSelfRef.soc_top__DOT__ram_rdata_valid;
    } else if ((0x20000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_ready = vlSelfRef.soc_top__DOT__gpio_mem_ready;
    }
    if (vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer) {
        vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode 
            = vlSelfRef.soc_top__DOT__cpu_mem_rdata;
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q 
            = vlSelfRef.soc_top__DOT__cpu_mem_rdata;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer = ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_ready) 
                                                  & (IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid));
    vlSelfRef.soc_top__DOT__cpu_mem_rdata = 0U;
    if ((0U == (0xffff0000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_rdata = vlSelfRef.soc_top__DOT__rom_rdata;
    } else if ((0x10000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_rdata = vlSelfRef.soc_top__DOT__ram_rdata;
    } else if ((0x20000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_rdata = ((0x20000023U 
                                                  == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                                                  ? (IData)(vlSelfRef.gpio_pin_in)
                                                  : 
                                                 ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)
                                                   ? 
                                                  ((0x20000024U 
                                                    == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                                                    ? (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__ddrb)
                                                    : 
                                                   ((0x20000025U 
                                                     == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                                                     ? (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__portb)
                                                     : 0U))
                                                   : 0U));
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer)
            ? vlSelfRef.soc_top__DOT__cpu_mem_rdata
            : vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q);
    if ((0U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wdata 
            = vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2;
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb = 0x0fU;
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
            = vlSelfRef.soc_top__DOT__cpu_mem_rdata;
    } else if ((1U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wdata 
            = ((vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                << 0x00000010U) | (0x0000ffffU & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2));
        if ((2U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)) {
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb = 0x0cU;
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
                = (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                   >> 0x10U);
        } else {
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb = 3U;
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
                = (0x0000ffffU & vlSelfRef.soc_top__DOT__cpu_mem_rdata);
        }
    } else if ((2U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wdata 
            = ((vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                << 0x00000018U) | ((0x00ff0000U & (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                                                   << 0x00000010U)) 
                                   | ((0x0000ff00U 
                                       & (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                                          << 8U)) | 
                                      (0x000000ffU 
                                       & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2))));
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb 
            = (0x0000000fU & ((IData)(1U) << (3U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)));
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
            = ((2U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)
                ? ((1U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)
                    ? (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                       >> 0x18U) : (0x000000ffU & (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                                                   >> 0x10U)))
                : ((1U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)
                    ? (0x000000ffU & (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                                      >> 8U)) : (0x000000ffU 
                                                 & vlSelfRef.soc_top__DOT__cpu_mem_rdata)));
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_instr 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_ascii_instr;
    if (vlSelfRef.soc_top__DOT__cpu__DOT__dbg_next) {
        if (vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger_q) {
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_opcode 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_opcode;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs1 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rs1;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rd 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rd;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs2 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rs2;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_imm 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_imm;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_instr 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_ascii_instr;
        } else {
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_opcode 
                = ((3U == (3U & vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode))
                    ? vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode
                    : (0x0000ffffU & vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode));
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs1 
                = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rd 
                = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rd;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs2 
                = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_imm 
                = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_instr 
                = vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr;
        }
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst 
        = __Vdly__soc_top__DOT__cpu__DOT__mem_do_rinst;
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_read = 
        ((IData)(vlSelfRef.rst_n) & ((~ (0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) 
                                     & ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
                                        | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch) 
                                           | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata)))));
    vlSelfRef.soc_top__DOT__cpu__DOT____VdfgRegularize_h233421b0_0_0 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata) 
              | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata)));
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_done = ((IData)(vlSelfRef.rst_n) 
                                                  & (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer) 
                                                      & ((0U 
                                                          != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state)) 
                                                         & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT____VdfgRegularize_h233421b0_0_0))) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state)) 
                                                        & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst))));
}

void Vsoc_top___024root___eval_nba(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_nba\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsoc_top___024root___nba_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
}

void Vsoc_top___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vsoc_top___024root___eval_phase__act(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_phase__act\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vsoc_top___024root___eval_triggers__act(vlSelf);
    Vsoc_top___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vsoc_top___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vsoc_top___024root___eval_phase__nba(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_phase__nba\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vsoc_top___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vsoc_top___024root___eval_nba(vlSelf);
        Vsoc_top___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vsoc_top___024root___eval(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vsoc_top___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
#endif
            VL_FATAL_MT("../rtl/../rtl/soc_top.v", 6, "", "Input combinational region did not converge after 100 tries");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
    } while (Vsoc_top___024root___eval_phase__ico(vlSelf));
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vsoc_top___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("../rtl/../rtl/soc_top.v", 6, "", "NBA region did not converge after 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vsoc_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("../rtl/../rtl/soc_top.v", 6, "", "Active region did not converge after 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
        } while (Vsoc_top___024root___eval_phase__act(vlSelf));
    } while (Vsoc_top___024root___eval_phase__nba(vlSelf));
}

#ifdef VL_DEBUG
void Vsoc_top___024root___eval_debug_assertions(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_debug_assertions\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");
    }
}
#endif  // VL_DEBUG
