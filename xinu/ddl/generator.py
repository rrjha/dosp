def generate(tree):
    
    c_header = "#include<stdio.h>\n"
    c_end = "return 0;\n}"
    root = tree.getroot()
    rchild =root[0]
    interface = rchild.find('Interface')
    for signal in interface.findall('Signal'):
        method = signal.find('Method_name')
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

