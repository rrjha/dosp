def generate(tree):
    
    c_header = "#include<stdio.h>\n#include<stdlib.h>\n"
    c_end = "return 0;\n}"
    c_stmt = "fprintf(stderr, """""outside range! Exiting.." """");\n"


    root = tree.getroot()
    rchild =root[0]
    interface = rchild.find('Interface')
    for signal in interface.findall('Signal'):
        method = signal.find('Method_name')
        parameter = signal.find('Parameter')
        if parameter is not None:
            id1 = parameter.get('id')
            chars = parameter.findall('Parameter_characterization')
            lb = chars[0].text
            ub = chars[1].text
            c_cpp = c_header + "int do_" + method.text + "("+ "int " + id1 + "){\n" + "if("+id1+"<"+lb+")||("+id1+">"+ub+"){\n"+c_stmt+"exit(-1);\n}\n" +method.text + "(" + "int " + id1 + ");\n" + c_end
            c_prototype = "int do_" + method.text + "("+ "int " + id1 + ");"
        else:
            c_cpp = c_header + "int do_" + method.text + "(){\n" + method.text + "();\n" + c_end
            c_prototype = "int do_" + method.text + "();"
        fileC = method.text + ".c"
        fC = open(fileC, 'w+')
        fC.write(c_cpp)
        fC.close()
        fileH = method.text + ".h"
        fH = open(fileH, 'w+')
        fH.write(c_prototype)
        fH.close()

