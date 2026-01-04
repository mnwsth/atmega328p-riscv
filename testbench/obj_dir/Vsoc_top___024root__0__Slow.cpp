// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsoc_top.h for the primary calling header

#include "Vsoc_top__pch.h"

VL_ATTR_COLD void Vsoc_top___024root___eval_static(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_static\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

VL_ATTR_COLD void Vsoc_top___024root___eval_initial__TOP(Vsoc_top___024root* vlSelf);
VL_ATTR_COLD void Vsoc_top___024root____Vm_traceActivitySetAll(Vsoc_top___024root* vlSelf);

VL_ATTR_COLD void Vsoc_top___024root___eval_initial(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_initial\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vsoc_top___024root___eval_initial__TOP(vlSelf);
    Vsoc_top___024root____Vm_traceActivitySetAll(vlSelf);
}

VL_ATTR_COLD void Vsoc_top___024root___eval_initial__TOP(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_initial__TOP\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    VL_READMEM_N(true, 32, 65536, 0, "firmware.mem"s
                 ,  &(vlSelfRef.soc_top__DOT__program_rom__DOT__mem)
                 , 0, ~0ULL);
    vlSelfRef.soc_top__DOT__data_ram__DOT__i = 0U;
    while (VL_GTS_III(32, 0x00001000U, vlSelfRef.soc_top__DOT__data_ram__DOT__i)) {
        vlSelfRef.soc_top__DOT__data_ram__DOT__mem[(0x00000fffU 
                                                    & vlSelfRef.soc_top__DOT__data_ram__DOT__i)] = 0U;
        vlSelfRef.soc_top__DOT__data_ram__DOT__i = 
            ((IData)(1U) + vlSelfRef.soc_top__DOT__data_ram__DOT__i);
    }
}

VL_ATTR_COLD void Vsoc_top___024root___eval_final(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_final\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsoc_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vsoc_top___024root___eval_phase__stl(Vsoc_top___024root* vlSelf);

VL_ATTR_COLD void Vsoc_top___024root___eval_settle(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_settle\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vsoc_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("../rtl/../rtl/soc_top.v", 6, "", "Settle region did not converge after 100 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
    } while (Vsoc_top___024root___eval_phase__stl(vlSelf));
}

VL_ATTR_COLD void Vsoc_top___024root___eval_triggers__stl(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_triggers__stl\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered
                                      [0U]) | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
    vlSelfRef.__VstlFirstIteration = 0U;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsoc_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
}

VL_ATTR_COLD bool Vsoc_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsoc_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vsoc_top___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vsoc_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___trigger_anySet__stl\n"); );
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

VL_ATTR_COLD void Vsoc_top___024root___stl_sequent__TOP__0(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___stl_sequent__TOP__0\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*7:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelfRef.gpio_pin_out = vlSelfRef.soc_top__DOT__gpio_inst__DOT__portb;
    vlSelfRef.gpio_pin_dir = vlSelfRef.soc_top__DOT__gpio_inst__DOT__ddrb;
    vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1 = 
        ((0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1))
          ? vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs
         [vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1]
          : 0U);
    vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2 = 
        ((0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2))
          ? vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs
         [vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2]
          : 0U);
    vlSelfRef.soc_top__DOT__program_rom__DOT__ce = 
        ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid) 
         & (0U == (0xffff0000U & vlSelfRef.soc_top__DOT__cpu_mem_addr)));
    vlSelfRef.soc_top__DOT__cpu__DOT__launch_next_insn 
        = ((0x40U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state)) 
           & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger));
    vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_write = 0U;
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
    vlSelfRef.soc_top__DOT__cpu__DOT__is_rdcycle_rdcycleh_rdinstr_rdinstrh 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycle) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycleh) 
              | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstr) 
                 | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstrh))));
    vlSelfRef.soc_top__DOT__cpu__DOT__next_pc = (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch) 
                                                  & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__latched_store))
                                                  ? 
                                                 (0xfffffffeU 
                                                  & vlSelfRef.soc_top__DOT__cpu__DOT__reg_out)
                                                  : vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc);
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_imm 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_imm;
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_opcode 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_opcode;
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs1 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rs1;
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs2 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rs2;
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rd = vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rd;
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
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_read = 
        ((IData)(vlSelfRef.rst_n) & ((~ (0U != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state))) 
                                     & ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
                                        | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch) 
                                           | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata)))));
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
    vlSelfRef.soc_top__DOT__data_ram__DOT__ce = ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid) 
                                                 & (0x10000000U 
                                                    == 
                                                    (0xfffff000U 
                                                     & vlSelfRef.soc_top__DOT__cpu_mem_addr)));
    vlSelfRef.soc_top__DOT__cpu__DOT____VdfgRegularize_h233421b0_0_0 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst) 
           | ((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata) 
              | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata)));
    vlSelfRef.soc_top__DOT__cpu_mem_ready = 0U;
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_eq = (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                                                == vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_lts = VL_LTS_III(32, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1, vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_ltu = (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                                                 < vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2);
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
    vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_valid 
        = ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid) 
           & (0x20000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr)));
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
    if ((0U == (0xffff0000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_ready = vlSelfRef.soc_top__DOT__rom_rdata_valid;
    } else if ((0x10000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_ready = vlSelfRef.soc_top__DOT__ram_rdata_valid;
    } else if ((0x20000000U == (0xfffff000U & vlSelfRef.soc_top__DOT__cpu_mem_addr))) {
        vlSelfRef.soc_top__DOT__cpu_mem_ready = vlSelfRef.soc_top__DOT__gpio_mem_ready;
    }
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer = ((IData)(vlSelfRef.soc_top__DOT__cpu_mem_ready) 
                                                  & (IData)(vlSelfRef.soc_top__DOT__cpu_mem_valid));
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0 = 0U;
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
    vlSelfRef.soc_top__DOT__cpu__DOT__alu_out = 0U;
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
    vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_instr 
        = vlSelfRef.soc_top__DOT__cpu__DOT__q_ascii_instr;
    if (vlSelfRef.soc_top__DOT__cpu__DOT__dbg_next) {
        if (vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger_q) {
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_imm 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_imm;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_opcode 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_opcode;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs1 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rs1;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs2 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rs2;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rd 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rd;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_instr 
                = vlSelfRef.soc_top__DOT__cpu__DOT__cached_ascii_instr;
        } else {
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_imm 
                = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_opcode 
                = ((3U == (3U & vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode))
                    ? vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode
                    : (0x0000ffffU & vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode));
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs1 
                = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs2 
                = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rd 
                = vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rd;
            vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_instr 
                = vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr;
        }
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
    vlSelfRef.soc_top__DOT__cpu__DOT__mem_done = ((IData)(vlSelfRef.rst_n) 
                                                  & (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer) 
                                                      & ((0U 
                                                          != (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state)) 
                                                         & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT____VdfgRegularize_h233421b0_0_0))) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state)) 
                                                        & (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst))));
    vlSelfRef.__VdfgRegularize_hebeb780c_0_0 = ((0x20000024U 
                                                 == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr) 
                                                | (0x20000025U 
                                                   == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr));
    vlSelfRef.soc_top__DOT__gpio_inst__DOT__gpio_sel 
        = ((0x20000023U == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr) 
           | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0));
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
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb = 0x0fU;
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wdata 
            = vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2;
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
            = vlSelfRef.soc_top__DOT__cpu_mem_rdata;
    } else if ((1U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize))) {
        if ((2U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)) {
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb = 0x0cU;
            if ((2U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)) {
                vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
                    = (vlSelfRef.soc_top__DOT__cpu_mem_rdata 
                       >> 0x10U);
            }
        } else {
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb = 3U;
            vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word 
                = (0x0000ffffU & vlSelfRef.soc_top__DOT__cpu_mem_rdata);
        }
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wdata 
            = ((vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                << 0x00000010U) | (0x0000ffffU & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2));
    } else if ((2U == (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize))) {
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb 
            = (0x0000000fU & ((IData)(1U) << (3U & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1)));
        vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wdata 
            = ((vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                << 0x00000018U) | ((0x00ff0000U & (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                                                   << 0x00000010U)) 
                                   | ((0x0000ff00U 
                                       & (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2 
                                          << 8U)) | 
                                      (0x000000ffU 
                                       & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2))));
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

VL_ATTR_COLD void Vsoc_top___024root___eval_stl(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_stl\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vsoc_top___024root___stl_sequent__TOP__0(vlSelf);
        Vsoc_top___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

VL_ATTR_COLD bool Vsoc_top___024root___eval_phase__stl(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___eval_phase__stl\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vsoc_top___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = Vsoc_top___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vsoc_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vsoc_top___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsoc_top___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vsoc_top___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vsoc_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsoc_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vsoc_top___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vsoc_top___024root____Vm_traceActivitySetAll(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root____Vm_traceActivitySetAll\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vm_traceActivity[0U] = 1U;
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
}

VL_ATTR_COLD void Vsoc_top___024root___ctor_var_reset(Vsoc_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root___ctor_var_reset\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->gpio_pin_in = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 9677711068521919939ull);
    vlSelf->gpio_pin_out = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16418222110546629919ull);
    vlSelf->gpio_pin_dir = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16844707481776691213ull);
    vlSelf->soc_top__DOT__cpu_mem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 279645297158534546ull);
    vlSelf->soc_top__DOT__cpu_mem_instr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10804333794415694893ull);
    vlSelf->soc_top__DOT__cpu_mem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5349403342960309994ull);
    vlSelf->soc_top__DOT__cpu_mem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1376179607237716271ull);
    vlSelf->soc_top__DOT__cpu_mem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5431324725490784475ull);
    vlSelf->soc_top__DOT__cpu_mem_wstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13866203297743740746ull);
    vlSelf->soc_top__DOT__cpu_mem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5673831290382325951ull);
    vlSelf->soc_top__DOT__cpu_trap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7620143762382528882ull);
    vlSelf->soc_top__DOT__rom_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3513385243665353387ull);
    vlSelf->soc_top__DOT__rom_rdata_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18054625396363509178ull);
    vlSelf->soc_top__DOT__ram_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17225098428525558701ull);
    vlSelf->soc_top__DOT__ram_rdata_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9812986133167268488ull);
    vlSelf->soc_top__DOT__gpio_mem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4222530810631000172ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_la_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15515703913440157284ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_la_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2150282339782268592ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_la_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1077519497307168601ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_la_wstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 3383524088313565720ull);
    vlSelf->soc_top__DOT__cpu__DOT__pcpi_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12808723200727432201ull);
    vlSelf->soc_top__DOT__cpu__DOT__pcpi_insn = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3679691126463621171ull);
    vlSelf->soc_top__DOT__cpu__DOT__eoi = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9952704563024182392ull);
    vlSelf->soc_top__DOT__cpu__DOT__trace_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 915568025183820520ull);
    vlSelf->soc_top__DOT__cpu__DOT__trace_data = VL_SCOPED_RAND_RESET_Q(36, __VscopeHash, 7980524990515031819ull);
    vlSelf->soc_top__DOT__cpu__DOT__count_cycle = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 6339120327472451108ull);
    vlSelf->soc_top__DOT__cpu__DOT__count_instr = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5356222810119662905ull);
    vlSelf->soc_top__DOT__cpu__DOT__reg_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14245529096271108516ull);
    vlSelf->soc_top__DOT__cpu__DOT__reg_next_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12031433105162702531ull);
    vlSelf->soc_top__DOT__cpu__DOT__reg_op1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12695907250281018796ull);
    vlSelf->soc_top__DOT__cpu__DOT__reg_op2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6784521860568016359ull);
    vlSelf->soc_top__DOT__cpu__DOT__reg_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10018986010857807034ull);
    vlSelf->soc_top__DOT__cpu__DOT__reg_sh = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 245433577523180215ull);
    vlSelf->soc_top__DOT__cpu__DOT__next_insn_opcode = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11711751580369902381ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_insn_opcode = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12018134871088742967ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_insn_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7259485464036885133ull);
    vlSelf->soc_top__DOT__cpu__DOT__next_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16230374212497262018ull);
    vlSelf->soc_top__DOT__cpu__DOT__irq_delay = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8732345370163947263ull);
    vlSelf->soc_top__DOT__cpu__DOT__irq_active = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9728128298606359545ull);
    vlSelf->soc_top__DOT__cpu__DOT__irq_mask = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13047283042191532359ull);
    vlSelf->soc_top__DOT__cpu__DOT__irq_pending = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2091508395752117697ull);
    vlSelf->soc_top__DOT__cpu__DOT__timer = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14776134144262670423ull);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->soc_top__DOT__cpu__DOT__cpuregs[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9426389682786911367ull);
    }
    vlSelf->soc_top__DOT__cpu__DOT__i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7346490268463421901ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_state = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 13644001615326198262ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_wordsize = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15956756032242152226ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_rdata_word = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8728779392755633256ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_rdata_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11453526240727236261ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_do_prefetch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13962858137249479896ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_do_rinst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6954966044890255330ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_do_rdata = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1491792652511521712ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_do_wdata = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11949583626186840187ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_xfer = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18020375289757882809ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_la_secondword = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14582597583277354624ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_la_firstword_reg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4455031627367791689ull);
    vlSelf->soc_top__DOT__cpu__DOT__last_mem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7590244231033108172ull);
    vlSelf->soc_top__DOT__cpu__DOT__prefetched_high_word = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13355291908276315102ull);
    vlSelf->soc_top__DOT__cpu__DOT__clear_prefetched_high_word = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7353312288051932644ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_16bit_buffer = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 78647230690591795ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6948074287119110926ull);
    vlSelf->soc_top__DOT__cpu__DOT__mem_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5932345592668112155ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_lui = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11347969212342468722ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_auipc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15567058619215204095ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_jal = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4456485101477330692ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_jalr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9991191226309878563ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_beq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12884522860298631206ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_bne = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14329777573057794180ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_blt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13046019363497714619ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_bge = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10143760089886908463ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_bltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13846386257356125466ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_bgeu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13490092235372879882ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_lb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11399076057855566506ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_lh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16771891535863236691ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2299829371132442304ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_lbu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 406102706101889727ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_lhu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14115506733618182685ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_sb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10566732127928408592ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_sh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 736725889530562439ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17229555618312075154ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_addi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15508046708729248434ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_slti = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 709403342113089075ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_sltiu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17941055015120183268ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_xori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13363833557690178536ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_ori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1263342011146756250ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_andi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10003211110484887281ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_slli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3974430504565309599ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_srli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13273823853822290160ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_srai = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11692970498372887931ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13416291301462318124ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5030316969430453075ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_sll = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5467114161012725165ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_slt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1795062754003224870ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_sltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14353487105635273767ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_xor = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17505455059292413324ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_srl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17756129838723311279ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_sra = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16372243285059692169ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_or = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1259024595232788246ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_and = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1274860312908272472ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_rdcycle = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12933191315449732637ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_rdcycleh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6779070663607075825ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_rdinstr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6841776901382085376ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_rdinstrh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11558052404427874043ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_ecall_ebreak = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13360981826220102427ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_fence = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18035549205869489606ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_getq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9999199982193795254ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_setq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5982204139436059398ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_maskirq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4882266102609604408ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_waitirq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5711024595686515743ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_timer = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6844209240683477012ull);
    vlSelf->soc_top__DOT__cpu__DOT__instr_trap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12891091700482610621ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoded_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9354947443000909946ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoded_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 14838700569559952169ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoded_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 4150447207648550422ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoded_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2412882265023948879ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoded_imm_j = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2588376606978104736ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoder_trigger = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6040830618959215290ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoder_trigger_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10083295801170985220ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoder_pseudo_trigger = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16048097431189479187ull);
    vlSelf->soc_top__DOT__cpu__DOT__decoder_pseudo_trigger_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4054835762719253819ull);
    vlSelf->soc_top__DOT__cpu__DOT__compressed_instr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12109306536474491470ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_lui_auipc_jal = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14488855509057206811ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16934783834390609897ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_slli_srli_srai = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 59886016128657005ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_jalr_addi_slti_sltiu_xori_ori_andi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16214619792058010404ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_sb_sh_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14524642100092919625ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_sll_srl_sra = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5563861440104314822ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_lui_auipc_jal_jalr_addi_add_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7362782962504223283ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_slti_blt_slt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2700761788616358837ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_sltiu_bltu_sltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1744393102819195815ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13787010310708617214ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_lbu_lhu_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7663562337897753938ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_alu_reg_imm = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18311423022990147248ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_alu_reg_reg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6927763564196218625ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_compare = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7091173380409985667ull);
    vlSelf->soc_top__DOT__cpu__DOT__is_rdcycle_rdcycleh_rdinstr_rdinstrh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6520831620234706726ull);
    vlSelf->soc_top__DOT__cpu__DOT__new_ascii_instr = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8991024556799734393ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_ascii_instr = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 10634986864232484315ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_insn_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8788823951855634243ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_insn_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 15967222109426888422ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_insn_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 3869224662672756637ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_insn_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 14771242372009956918ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_rs1val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3561189175981868006ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_rs2val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10957401415675365260ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_rs1val_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14183527975765739379ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_rs2val_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7174405999951475571ull);
    vlSelf->soc_top__DOT__cpu__DOT__q_ascii_instr = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8237194976852274943ull);
    vlSelf->soc_top__DOT__cpu__DOT__q_insn_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7303034578108261978ull);
    vlSelf->soc_top__DOT__cpu__DOT__q_insn_opcode = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6351578672235855562ull);
    vlSelf->soc_top__DOT__cpu__DOT__q_insn_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 2892600758257669214ull);
    vlSelf->soc_top__DOT__cpu__DOT__q_insn_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16357407396744810262ull);
    vlSelf->soc_top__DOT__cpu__DOT__q_insn_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1640159428979960946ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13062329090478318386ull);
    vlSelf->soc_top__DOT__cpu__DOT__launch_next_insn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14614892424543100469ull);
    vlSelf->soc_top__DOT__cpu__DOT__dbg_valid_insn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9754712000336455160ull);
    vlSelf->soc_top__DOT__cpu__DOT__cached_ascii_instr = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 17323065296607847184ull);
    vlSelf->soc_top__DOT__cpu__DOT__cached_insn_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1253076163398752965ull);
    vlSelf->soc_top__DOT__cpu__DOT__cached_insn_opcode = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12789858383728029333ull);
    vlSelf->soc_top__DOT__cpu__DOT__cached_insn_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 8826999163935917806ull);
    vlSelf->soc_top__DOT__cpu__DOT__cached_insn_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 140305072348111639ull);
    vlSelf->soc_top__DOT__cpu__DOT__cached_insn_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9722744858420547295ull);
    vlSelf->soc_top__DOT__cpu__DOT__cpu_state = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5806784081866965735ull);
    vlSelf->soc_top__DOT__cpu__DOT__irq_state = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9584725702381038626ull);
    VL_SCOPED_RAND_RESET_W(128, vlSelf->soc_top__DOT__cpu__DOT__dbg_ascii_state, __VscopeHash, 9943539582389178965ull);
    vlSelf->soc_top__DOT__cpu__DOT__set_mem_do_rinst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 541887315129543114ull);
    vlSelf->soc_top__DOT__cpu__DOT__set_mem_do_rdata = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17732106468196509549ull);
    vlSelf->soc_top__DOT__cpu__DOT__set_mem_do_wdata = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15354357754800196827ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_store = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 536029399115501787ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_stalu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3847573979846554242ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_branch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16566011762675443728ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_compr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5761794460015064075ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_trace = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2858197133321808418ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_is_lu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9717019114937552758ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_is_lh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4553124630833615924ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_is_lb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9525031409274831129ull);
    vlSelf->soc_top__DOT__cpu__DOT__latched_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 4813483197162996603ull);
    vlSelf->soc_top__DOT__cpu__DOT__current_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4415090203141969669ull);
    vlSelf->soc_top__DOT__cpu__DOT__pcpi_timeout_counter = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5266093288669576258ull);
    vlSelf->soc_top__DOT__cpu__DOT__pcpi_timeout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3038399311191958037ull);
    vlSelf->soc_top__DOT__cpu__DOT__next_irq_pending = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10954018245291718781ull);
    vlSelf->soc_top__DOT__cpu__DOT__do_waitirq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8934975815388156200ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17788248579756354022ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_out_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17742544385434060071ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_out_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14523643482187354567ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_out_0_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9551724259981607634ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_wait = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3228314872315712489ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_wait_2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11770632118403256154ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_eq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15991004271729553631ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_ltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4645749281742389103ull);
    vlSelf->soc_top__DOT__cpu__DOT__alu_lts = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7160770220942907639ull);
    vlSelf->soc_top__DOT__cpu__DOT__clear_prefetched_high_word_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14318559322506447912ull);
    vlSelf->soc_top__DOT__cpu__DOT__cpuregs_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13639996400823888473ull);
    vlSelf->soc_top__DOT__cpu__DOT__cpuregs_wrdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2676248074843980580ull);
    vlSelf->soc_top__DOT__cpu__DOT__cpuregs_rs1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10448860471842390534ull);
    vlSelf->soc_top__DOT__cpu__DOT__cpuregs_rs2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1374678678462199068ull);
    vlSelf->soc_top__DOT__cpu__DOT____VdfgRegularize_h233421b0_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10192658266943765046ull);
    vlSelf->soc_top__DOT__program_rom__DOT__ce = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9584602189545606420ull);
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->soc_top__DOT__program_rom__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15993802321569251325ull);
    }
    vlSelf->soc_top__DOT__data_ram__DOT__ce = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8643227988419272017ull);
    vlSelf->soc_top__DOT__data_ram__DOT__we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1655221587735108338ull);
    vlSelf->soc_top__DOT__data_ram__DOT__addr = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 16923935801114701771ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->soc_top__DOT__data_ram__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10154069736612141014ull);
    }
    vlSelf->soc_top__DOT__data_ram__DOT__i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10861167878538850266ull);
    vlSelf->soc_top__DOT__gpio_inst__DOT__mem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17511970490795250500ull);
    vlSelf->soc_top__DOT__gpio_inst__DOT__mem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8394162957695253262ull);
    vlSelf->soc_top__DOT__gpio_inst__DOT__mem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15303220441442388555ull);
    vlSelf->soc_top__DOT__gpio_inst__DOT__mem_wstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14654268489613782187ull);
    vlSelf->soc_top__DOT__gpio_inst__DOT__ddrb = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4174734873882249154ull);
    vlSelf->soc_top__DOT__gpio_inst__DOT__portb = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10564737125387262271ull);
    vlSelf->soc_top__DOT__gpio_inst__DOT__gpio_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3913225353296974291ull);
    vlSelf->__VdfgRegularize_hebeb780c_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14076179427513946122ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
