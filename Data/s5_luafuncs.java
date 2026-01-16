//TODO write a description for this script
//@author 
//@category Windows
//@keybinding 
//@menupath 
//@toolbar 
//@runtime Java

import ghidra.app.script.GhidraScript;
import ghidra.program.model.sourcemap.*;
import ghidra.program.model.lang.protorules.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.lang.*;
import ghidra.program.model.pcode.*;
import ghidra.program.model.data.ISF.*;
import ghidra.program.model.util.*;
import ghidra.program.model.reloc.*;
import ghidra.program.model.data.*;
import ghidra.program.model.block.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.scalar.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;
import java.util.*;

public class s5_luafuncs extends GhidraScript {

    public void run() throws Exception {
        //DataType dt = getDataTypes("EScr::StaticFuncList")[0];
        //Address addr = toAddr(0x0084efac);
        //createData(addr, dt);
        FunctionManager fm = currentProgram.getFunctionManager();
        DataType dt_int = getDataTypes("int")[0];
        DataType dt_state = getDataTypes("lua_State *")[0];
        ArrayList<Variable> params = new ArrayList<Variable>();
        params.add(new ParameterImpl("L", dt_state, currentProgram));

        try(BufferedReader br = new BufferedReader(new FileReader("/home/mcb/ghidra_scripts/s5_luafuncs.txt"))) {
            for(String line; (line = br.readLine()) != null; ) {
                String[] l = line.split("\\|");
                if (l[1].equals("type")) {
                    DataType dt = getDataTypes(l[2])[0];
                    Address addr = toAddr(Integer.parseInt(l[0]));
                    DataUtilities.createData(currentProgram, addr, dt, 0, DataUtilities.ClearDataMode.CLEAR_ALL_CONFLICT_DATA);
                }
                else if (l[1].equals("func")) {
                    Address addr = toAddr(Integer.parseInt(l[0]));
                    Function f = fm.getFunctionAt(addr);
                    if (f != null) {
                        f.setName(l[2], SourceType.IMPORTED);
                    }
                    else {
                        f = fm.createFunction(l[2], addr, ghidra.app.cmd.function.CreateFunctionCmd.getFunctionBody(currentProgram, addr), SourceType.IMPORTED);
                        println(l[2]);
                    }
                    f.setCallingConvention("__cdecl");
                    f.setCustomVariableStorage(false);
                    f.setReturnType(dt_int, SourceType.IMPORTED);
                    f.replaceParameters(params, Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.IMPORTED);
                }
            }
        }
    }

}
