// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vsoc_top__Syms.h"


void Vsoc_top___024root__trace_chg_0_sub_0(Vsoc_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vsoc_top___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root__trace_chg_0\n"); );
    // Body
    Vsoc_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vsoc_top___024root*>(voidSelf);
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    Vsoc_top___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vsoc_top___024root__trace_chg_0_sub_0(Vsoc_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root__trace_chg_0_sub_0\n"); );
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[0U]))) {
        bufp->chgIData(oldp+0,(vlSelfRef.soc_top__DOT__data_ram__DOT__i),32);
    }
    if (VL_UNLIKELY(((vlSelfRef.__Vm_traceActivity[1U] 
                      | vlSelfRef.__Vm_traceActivity
                      [2U])))) {
        bufp->chgIData(oldp+1,(vlSelfRef.soc_top__DOT__cpu_mem_rdata),32);
        bufp->chgBit(oldp+2,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_read));
        bufp->chgBit(oldp+3,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_write));
        bufp->chgIData(oldp+4,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_word),32);
        bufp->chgBit(oldp+5,(vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word));
        bufp->chgIData(oldp+6,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_latched_noshuffle),32);
        bufp->chgBit(oldp+7,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_done));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[2U]))) {
        bufp->chgBit(oldp+8,(vlSelfRef.soc_top__DOT__cpu_mem_valid));
        bufp->chgBit(oldp+9,(vlSelfRef.soc_top__DOT__cpu_mem_instr));
        bufp->chgBit(oldp+10,(vlSelfRef.soc_top__DOT__cpu_mem_ready));
        bufp->chgIData(oldp+11,(vlSelfRef.soc_top__DOT__cpu_mem_addr),32);
        bufp->chgIData(oldp+12,(vlSelfRef.soc_top__DOT__cpu_mem_wdata),32);
        bufp->chgCData(oldp+13,(vlSelfRef.soc_top__DOT__cpu_mem_wstrb),4);
        bufp->chgBit(oldp+14,(vlSelfRef.soc_top__DOT__cpu_trap));
        bufp->chgBit(oldp+15,(vlSelfRef.soc_top__DOT__program_rom__DOT__ce));
        bufp->chgSData(oldp+16,(((IData)(vlSelfRef.soc_top__DOT__program_rom__DOT__ce)
                                  ? (0x00003fffU & 
                                     (vlSelfRef.soc_top__DOT__cpu_mem_addr 
                                      >> 2U)) : 0U)),16);
        bufp->chgIData(oldp+17,(vlSelfRef.soc_top__DOT__rom_rdata),32);
        bufp->chgBit(oldp+18,(vlSelfRef.soc_top__DOT__rom_rdata_valid));
        bufp->chgBit(oldp+19,(vlSelfRef.soc_top__DOT__data_ram__DOT__ce));
        bufp->chgBit(oldp+20,(vlSelfRef.soc_top__DOT__data_ram__DOT__we));
        bufp->chgSData(oldp+21,(vlSelfRef.soc_top__DOT__data_ram__DOT__addr),12);
        bufp->chgIData(oldp+22,(vlSelfRef.soc_top__DOT__ram_rdata),32);
        bufp->chgBit(oldp+23,(vlSelfRef.soc_top__DOT__ram_rdata_valid));
        bufp->chgBit(oldp+24,(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_valid));
        bufp->chgIData(oldp+25,(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr),32);
        bufp->chgIData(oldp+26,(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wdata),32);
        bufp->chgCData(oldp+27,(vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_wstrb),4);
        bufp->chgBit(oldp+28,(vlSelfRef.soc_top__DOT__gpio_mem_ready));
        bufp->chgIData(oldp+29,((((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch) 
                                  | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst))
                                  ? (0xfffffffcU & vlSelfRef.soc_top__DOT__cpu__DOT__next_pc)
                                  : (0xfffffffcU & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1))),32);
        bufp->chgIData(oldp+30,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wdata),32);
        bufp->chgCData(oldp+31,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_wstrb),4);
        bufp->chgBit(oldp+32,(vlSelfRef.soc_top__DOT__cpu__DOT__pcpi_valid));
        bufp->chgIData(oldp+33,(vlSelfRef.soc_top__DOT__cpu__DOT__pcpi_insn),32);
        bufp->chgIData(oldp+34,(vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1),32);
        bufp->chgIData(oldp+35,(vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2),32);
        bufp->chgIData(oldp+36,(vlSelfRef.soc_top__DOT__cpu__DOT__eoi),32);
        bufp->chgBit(oldp+37,(vlSelfRef.soc_top__DOT__cpu__DOT__trace_valid));
        bufp->chgQData(oldp+38,(vlSelfRef.soc_top__DOT__cpu__DOT__trace_data),36);
        bufp->chgQData(oldp+40,(vlSelfRef.soc_top__DOT__cpu__DOT__count_cycle),64);
        bufp->chgQData(oldp+42,(vlSelfRef.soc_top__DOT__cpu__DOT__count_instr),64);
        bufp->chgIData(oldp+44,(vlSelfRef.soc_top__DOT__cpu__DOT__reg_pc),32);
        bufp->chgIData(oldp+45,(vlSelfRef.soc_top__DOT__cpu__DOT__reg_next_pc),32);
        bufp->chgIData(oldp+46,(vlSelfRef.soc_top__DOT__cpu__DOT__reg_out),32);
        bufp->chgCData(oldp+47,(vlSelfRef.soc_top__DOT__cpu__DOT__reg_sh),5);
        bufp->chgIData(oldp+48,(vlSelfRef.soc_top__DOT__cpu__DOT__next_insn_opcode),32);
        bufp->chgIData(oldp+49,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_opcode),32);
        bufp->chgIData(oldp+50,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_addr),32);
        bufp->chgIData(oldp+51,(vlSelfRef.soc_top__DOT__cpu__DOT__next_pc),32);
        bufp->chgBit(oldp+52,(vlSelfRef.soc_top__DOT__cpu__DOT__irq_delay));
        bufp->chgBit(oldp+53,(vlSelfRef.soc_top__DOT__cpu__DOT__irq_active));
        bufp->chgIData(oldp+54,(vlSelfRef.soc_top__DOT__cpu__DOT__irq_mask),32);
        bufp->chgIData(oldp+55,(vlSelfRef.soc_top__DOT__cpu__DOT__irq_pending),32);
        bufp->chgIData(oldp+56,(vlSelfRef.soc_top__DOT__cpu__DOT__timer),32);
        bufp->chgIData(oldp+57,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[0]),32);
        bufp->chgIData(oldp+58,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[1]),32);
        bufp->chgIData(oldp+59,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[2]),32);
        bufp->chgIData(oldp+60,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[3]),32);
        bufp->chgIData(oldp+61,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[4]),32);
        bufp->chgIData(oldp+62,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[5]),32);
        bufp->chgIData(oldp+63,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[6]),32);
        bufp->chgIData(oldp+64,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[7]),32);
        bufp->chgIData(oldp+65,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[8]),32);
        bufp->chgIData(oldp+66,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[9]),32);
        bufp->chgIData(oldp+67,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[10]),32);
        bufp->chgIData(oldp+68,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[11]),32);
        bufp->chgIData(oldp+69,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[12]),32);
        bufp->chgIData(oldp+70,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[13]),32);
        bufp->chgIData(oldp+71,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[14]),32);
        bufp->chgIData(oldp+72,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[15]),32);
        bufp->chgIData(oldp+73,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[16]),32);
        bufp->chgIData(oldp+74,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[17]),32);
        bufp->chgIData(oldp+75,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[18]),32);
        bufp->chgIData(oldp+76,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[19]),32);
        bufp->chgIData(oldp+77,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[20]),32);
        bufp->chgIData(oldp+78,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[21]),32);
        bufp->chgIData(oldp+79,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[22]),32);
        bufp->chgIData(oldp+80,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[23]),32);
        bufp->chgIData(oldp+81,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[24]),32);
        bufp->chgIData(oldp+82,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[25]),32);
        bufp->chgIData(oldp+83,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[26]),32);
        bufp->chgIData(oldp+84,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[27]),32);
        bufp->chgIData(oldp+85,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[28]),32);
        bufp->chgIData(oldp+86,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[29]),32);
        bufp->chgIData(oldp+87,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[30]),32);
        bufp->chgIData(oldp+88,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs[31]),32);
        bufp->chgCData(oldp+89,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_state),2);
        bufp->chgCData(oldp+90,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_wordsize),2);
        bufp->chgIData(oldp+91,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_rdata_q),32);
        bufp->chgBit(oldp+92,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch));
        bufp->chgBit(oldp+93,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rinst));
        bufp->chgBit(oldp+94,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_rdata));
        bufp->chgBit(oldp+95,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_wdata));
        bufp->chgBit(oldp+96,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_xfer));
        bufp->chgBit(oldp+97,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_secondword));
        bufp->chgBit(oldp+98,(vlSelfRef.soc_top__DOT__cpu__DOT__mem_la_firstword_reg));
        bufp->chgBit(oldp+99,(vlSelfRef.soc_top__DOT__cpu__DOT__last_mem_valid));
        bufp->chgBit(oldp+100,(vlSelfRef.soc_top__DOT__cpu__DOT__prefetched_high_word));
        bufp->chgBit(oldp+101,(((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__mem_do_prefetch) 
                                | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT____VdfgRegularize_h233421b0_0_0))));
        bufp->chgBit(oldp+102,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lui));
        bufp->chgBit(oldp+103,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_auipc));
        bufp->chgBit(oldp+104,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jal));
        bufp->chgBit(oldp+105,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_jalr));
        bufp->chgBit(oldp+106,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_beq));
        bufp->chgBit(oldp+107,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bne));
        bufp->chgBit(oldp+108,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_blt));
        bufp->chgBit(oldp+109,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bge));
        bufp->chgBit(oldp+110,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bltu));
        bufp->chgBit(oldp+111,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_bgeu));
        bufp->chgBit(oldp+112,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lb));
        bufp->chgBit(oldp+113,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lh));
        bufp->chgBit(oldp+114,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lw));
        bufp->chgBit(oldp+115,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lbu));
        bufp->chgBit(oldp+116,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_lhu));
        bufp->chgBit(oldp+117,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sb));
        bufp->chgBit(oldp+118,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sh));
        bufp->chgBit(oldp+119,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sw));
        bufp->chgBit(oldp+120,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_addi));
        bufp->chgBit(oldp+121,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slti));
        bufp->chgBit(oldp+122,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltiu));
        bufp->chgBit(oldp+123,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_xori));
        bufp->chgBit(oldp+124,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_ori));
        bufp->chgBit(oldp+125,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_andi));
        bufp->chgBit(oldp+126,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slli));
        bufp->chgBit(oldp+127,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srli));
        bufp->chgBit(oldp+128,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srai));
        bufp->chgBit(oldp+129,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_add));
        bufp->chgBit(oldp+130,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sub));
        bufp->chgBit(oldp+131,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sll));
        bufp->chgBit(oldp+132,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_slt));
        bufp->chgBit(oldp+133,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sltu));
        bufp->chgBit(oldp+134,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_xor));
        bufp->chgBit(oldp+135,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srl));
        bufp->chgBit(oldp+136,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sra));
        bufp->chgBit(oldp+137,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_or));
        bufp->chgBit(oldp+138,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_and));
        bufp->chgBit(oldp+139,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycle));
        bufp->chgBit(oldp+140,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdcycleh));
        bufp->chgBit(oldp+141,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstr));
        bufp->chgBit(oldp+142,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_rdinstrh));
        bufp->chgBit(oldp+143,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_ecall_ebreak));
        bufp->chgBit(oldp+144,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_fence));
        bufp->chgBit(oldp+145,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_getq));
        bufp->chgBit(oldp+146,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_setq));
        bufp->chgBit(oldp+147,(vlSelfRef.soc_top__DOT__cpu__DOT__compressed_instr));
        bufp->chgBit(oldp+148,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_maskirq));
        bufp->chgBit(oldp+149,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_waitirq));
        bufp->chgBit(oldp+150,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_timer));
        bufp->chgBit(oldp+151,(vlSelfRef.soc_top__DOT__cpu__DOT__instr_trap));
        bufp->chgCData(oldp+152,(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rd),5);
        bufp->chgCData(oldp+153,(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs1),5);
        bufp->chgCData(oldp+154,(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_rs2),5);
        bufp->chgIData(oldp+155,(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm),32);
        bufp->chgIData(oldp+156,(vlSelfRef.soc_top__DOT__cpu__DOT__decoded_imm_j),32);
        bufp->chgBit(oldp+157,(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger));
        bufp->chgBit(oldp+158,(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_trigger_q));
        bufp->chgBit(oldp+159,(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger));
        bufp->chgBit(oldp+160,(vlSelfRef.soc_top__DOT__cpu__DOT__decoder_pseudo_trigger_q));
        bufp->chgBit(oldp+161,(vlSelfRef.soc_top__DOT__cpu__DOT__is_lui_auipc_jal));
        bufp->chgBit(oldp+162,(vlSelfRef.soc_top__DOT__cpu__DOT__is_lb_lh_lw_lbu_lhu));
        bufp->chgBit(oldp+163,(vlSelfRef.soc_top__DOT__cpu__DOT__is_slli_srli_srai));
        bufp->chgBit(oldp+164,(vlSelfRef.soc_top__DOT__cpu__DOT__is_jalr_addi_slti_sltiu_xori_ori_andi));
        bufp->chgBit(oldp+165,(vlSelfRef.soc_top__DOT__cpu__DOT__is_sb_sh_sw));
        bufp->chgBit(oldp+166,(vlSelfRef.soc_top__DOT__cpu__DOT__is_sll_srl_sra));
        bufp->chgBit(oldp+167,(vlSelfRef.soc_top__DOT__cpu__DOT__is_lui_auipc_jal_jalr_addi_add_sub));
        bufp->chgBit(oldp+168,(vlSelfRef.soc_top__DOT__cpu__DOT__is_slti_blt_slt));
        bufp->chgBit(oldp+169,(vlSelfRef.soc_top__DOT__cpu__DOT__is_sltiu_bltu_sltu));
        bufp->chgBit(oldp+170,(vlSelfRef.soc_top__DOT__cpu__DOT__is_beq_bne_blt_bge_bltu_bgeu));
        bufp->chgBit(oldp+171,(vlSelfRef.soc_top__DOT__cpu__DOT__is_lbu_lhu_lw));
        bufp->chgBit(oldp+172,(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_imm));
        bufp->chgBit(oldp+173,(vlSelfRef.soc_top__DOT__cpu__DOT__is_alu_reg_reg));
        bufp->chgBit(oldp+174,(vlSelfRef.soc_top__DOT__cpu__DOT__is_compare));
        bufp->chgBit(oldp+175,(vlSelfRef.soc_top__DOT__cpu__DOT__is_rdcycle_rdcycleh_rdinstr_rdinstrh));
        bufp->chgQData(oldp+176,(vlSelfRef.soc_top__DOT__cpu__DOT__new_ascii_instr),64);
        bufp->chgQData(oldp+178,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_instr),64);
        bufp->chgIData(oldp+180,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_imm),32);
        bufp->chgCData(oldp+181,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs1),5);
        bufp->chgCData(oldp+182,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rs2),5);
        bufp->chgCData(oldp+183,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_insn_rd),5);
        bufp->chgIData(oldp+184,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val),32);
        bufp->chgIData(oldp+185,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs2val),32);
        bufp->chgBit(oldp+186,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs1val_valid));
        bufp->chgBit(oldp+187,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_rs2val_valid));
        bufp->chgQData(oldp+188,(vlSelfRef.soc_top__DOT__cpu__DOT__q_ascii_instr),64);
        bufp->chgIData(oldp+190,(vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_imm),32);
        bufp->chgIData(oldp+191,(vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_opcode),32);
        bufp->chgCData(oldp+192,(vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rs1),5);
        bufp->chgCData(oldp+193,(vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rs2),5);
        bufp->chgCData(oldp+194,(vlSelfRef.soc_top__DOT__cpu__DOT__q_insn_rd),5);
        bufp->chgBit(oldp+195,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_next));
        bufp->chgBit(oldp+196,(vlSelfRef.soc_top__DOT__cpu__DOT__launch_next_insn));
        bufp->chgBit(oldp+197,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_valid_insn));
        bufp->chgQData(oldp+198,(vlSelfRef.soc_top__DOT__cpu__DOT__cached_ascii_instr),64);
        bufp->chgIData(oldp+200,(vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_imm),32);
        bufp->chgIData(oldp+201,(vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_opcode),32);
        bufp->chgCData(oldp+202,(vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rs1),5);
        bufp->chgCData(oldp+203,(vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rs2),5);
        bufp->chgCData(oldp+204,(vlSelfRef.soc_top__DOT__cpu__DOT__cached_insn_rd),5);
        bufp->chgCData(oldp+205,(vlSelfRef.soc_top__DOT__cpu__DOT__cpu_state),8);
        bufp->chgCData(oldp+206,(vlSelfRef.soc_top__DOT__cpu__DOT__irq_state),2);
        bufp->chgWData(oldp+207,(vlSelfRef.soc_top__DOT__cpu__DOT__dbg_ascii_state),128);
        bufp->chgBit(oldp+211,(vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_rinst));
        bufp->chgBit(oldp+212,(vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_rdata));
        bufp->chgBit(oldp+213,(vlSelfRef.soc_top__DOT__cpu__DOT__set_mem_do_wdata));
        bufp->chgBit(oldp+214,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_store));
        bufp->chgBit(oldp+215,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_stalu));
        bufp->chgBit(oldp+216,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_branch));
        bufp->chgBit(oldp+217,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_compr));
        bufp->chgBit(oldp+218,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_trace));
        bufp->chgBit(oldp+219,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lu));
        bufp->chgBit(oldp+220,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lh));
        bufp->chgBit(oldp+221,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_is_lb));
        bufp->chgCData(oldp+222,(vlSelfRef.soc_top__DOT__cpu__DOT__latched_rd),5);
        bufp->chgIData(oldp+223,(vlSelfRef.soc_top__DOT__cpu__DOT__current_pc),32);
        bufp->chgBit(oldp+224,(vlSelfRef.soc_top__DOT__cpu__DOT__pcpi_timeout));
        bufp->chgIData(oldp+225,(vlSelfRef.soc_top__DOT__cpu__DOT__next_irq_pending),32);
        bufp->chgBit(oldp+226,(vlSelfRef.soc_top__DOT__cpu__DOT__do_waitirq));
        bufp->chgIData(oldp+227,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_out),32);
        bufp->chgIData(oldp+228,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_q),32);
        bufp->chgBit(oldp+229,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0));
        bufp->chgBit(oldp+230,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_out_0_q));
        bufp->chgBit(oldp+231,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_wait));
        bufp->chgBit(oldp+232,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_wait_2));
        bufp->chgIData(oldp+233,(((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sub)
                                   ? (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                                      - vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2)
                                   : (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                                      + vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2))),32);
        bufp->chgIData(oldp+234,((vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                                  << (0x0000001fU & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2))),32);
        bufp->chgIData(oldp+235,((IData)((0x00000001ffffffffULL 
                                          & VL_SHIFTRS_QQI(33,33,5, 
                                                           (((QData)((IData)(
                                                                             (((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_sra) 
                                                                               | (IData)(vlSelfRef.soc_top__DOT__cpu__DOT__instr_srai)) 
                                                                              & (vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1 
                                                                                >> 0x0000001fU)))) 
                                                             << 0x00000020U) 
                                                            | (QData)((IData)(vlSelfRef.soc_top__DOT__cpu__DOT__reg_op1))), 
                                                           (0x0000001fU 
                                                            & vlSelfRef.soc_top__DOT__cpu__DOT__reg_op2))))),32);
        bufp->chgBit(oldp+236,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_eq));
        bufp->chgBit(oldp+237,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_ltu));
        bufp->chgBit(oldp+238,(vlSelfRef.soc_top__DOT__cpu__DOT__alu_lts));
        bufp->chgBit(oldp+239,(vlSelfRef.soc_top__DOT__cpu__DOT__clear_prefetched_high_word_q));
        bufp->chgBit(oldp+240,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_write));
        bufp->chgIData(oldp+241,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_wrdata),32);
        bufp->chgIData(oldp+242,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs1),32);
        bufp->chgIData(oldp+243,(vlSelfRef.soc_top__DOT__cpu__DOT__cpuregs_rs2),32);
        bufp->chgBit(oldp+244,((0U == (0xffff0000U 
                                       & vlSelfRef.soc_top__DOT__cpu_mem_addr))));
        bufp->chgBit(oldp+245,((0x10000000U == (0xfffff000U 
                                                & vlSelfRef.soc_top__DOT__cpu_mem_addr))));
        bufp->chgBit(oldp+246,((0x20000000U == (0xfffff000U 
                                                & vlSelfRef.soc_top__DOT__cpu_mem_addr))));
        bufp->chgSData(oldp+247,((0x00003fffU & (vlSelfRef.soc_top__DOT__cpu_mem_addr 
                                                 >> 2U))),16);
        bufp->chgSData(oldp+248,((0x00000fffU & (vlSelfRef.soc_top__DOT__cpu_mem_addr 
                                                 >> 2U))),12);
        bufp->chgCData(oldp+249,(vlSelfRef.soc_top__DOT__gpio_inst__DOT__ddrb),8);
        bufp->chgCData(oldp+250,(vlSelfRef.soc_top__DOT__gpio_inst__DOT__portb),8);
        bufp->chgBit(oldp+251,((0x20000023U == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)));
        bufp->chgBit(oldp+252,((0x20000024U == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)));
        bufp->chgBit(oldp+253,((0x20000025U == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)));
        bufp->chgBit(oldp+254,(vlSelfRef.soc_top__DOT__gpio_inst__DOT__gpio_sel));
    }
    bufp->chgBit(oldp+255,(vlSelfRef.clk));
    bufp->chgBit(oldp+256,(vlSelfRef.rst_n));
    bufp->chgCData(oldp+257,(vlSelfRef.gpio_pin_in),8);
    bufp->chgCData(oldp+258,(vlSelfRef.gpio_pin_out),8);
    bufp->chgCData(oldp+259,(vlSelfRef.gpio_pin_dir),8);
    bufp->chgIData(oldp+260,(((0x20000023U == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                               ? (IData)(vlSelfRef.gpio_pin_in)
                               : ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)
                                   ? ((0x20000024U 
                                       == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                                       ? (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__ddrb)
                                       : ((0x20000025U 
                                           == vlSelfRef.soc_top__DOT__gpio_inst__DOT__mem_addr)
                                           ? (IData)(vlSelfRef.soc_top__DOT__gpio_inst__DOT__portb)
                                           : 0U)) : 0U))),32);
}

void Vsoc_top___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsoc_top___024root__trace_cleanup\n"); );
    // Body
    Vsoc_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vsoc_top___024root*>(voidSelf);
    Vsoc_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
}
