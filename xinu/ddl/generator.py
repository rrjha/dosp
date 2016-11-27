def generate(tree):
    
    c_header = "#include<xinu.h>\n"
    c_end = "return res;\n}"
    


    root = tree.getroot()
    rchild =root[0]
    interface = rchild.find('Interface')
    for signal in interface.findall('Signal'):
        method = signal.find('Method_name')
        optype = signal.find('Operation')
        if optype.text == 'Output':
            c_stmt = 'printf('+'"'+'Outside pin range..'+method.text+'"'+');\n'+'return SYSERR;\n'
            parameter = signal.find('Parameter')
            if parameter is not None:
                id1 = parameter.find('Parameter_value')
                a = id1.text
                if '-' in a:
                    a1, a2 = a.split('-')
                    b = "E"+a1+"_"+a2
                else:
                    b = a
                    a2 = a
                chars = parameter.findall('Parameter_characterization')
                lb = chars[0].text
                ub = chars[1].text
                c_cpp = c_header+"int32 do_"+method.text+"(){\nint32 p="+a2+", res;\nif((p<"+lb+")||(p>"+ub+")){\n"+c_stmt+"}\nres = " +method.text + "("+b+");\n" + c_end
                c_prototype = "int32 do_" + method.text + "();"
            else:
                c_cpp = c_header+"int32 do_"+method.text+"(){\nint32 res;\nres = "+method.text+"();\n"+c_end
                c_prototype = "int32 do_"+method.text+"();"
        else:
            parameter = signal.find('Parameter')
            ptype = parameter.find('Parameter_name').text
            pvalue = parameter.find('Parameter_value').text

            if pvalue == 'ACCEL':
                c_cpp = c_header+"int32 do_accel_"+method.text+"(){\nint32 res;\nres = "+method.text+"("+pvalue+");\n"+c_end
                c_prototype = "int32 do_accel_"+method.text+"();"

            elif pvalue == 'accel_d':
                c_cpp = c_header+"int32 do_accel_"+method.text+"("+ptype+" *"+pvalue+"){\nint32 res;\nres = "+method.text+"("+pvalue+");\n"+"return res;\n}"
                c_prototype = "int32 do_accel_"+method.text+"("+ptype+" *"+pvalue+");"

            else:
                c_cpp = c_header+"int32 do_"+method.text+"("+ptype+" *"+pvalue+"){\nint32 res;\nres = "+method.text+"("+pvalue+");\n"+"return res;\n}"
                c_prototype = "int32 do_"+method.text+"("+ptype+" *"+pvalue+");"

        fileC = '/Users/ankitsingh/project/dosp/xinu/ddl/test_c/do_'+method.text+'.c'
        fC = open(fileC, 'w+')
        fC.write(c_cpp)
        fC.close()

        fileH = 'do_'+method.text+'.h'
        fdir = '/Users/ankitsingh/project/dosp/xinu/ddl/test_h/'+fileH+'.h'
        fH = open(fileH, 'w+')
        fH.write(c_prototype)
        fH.close()

        fhead = '#include <'+fileH+'>'
        fx = open('/Users/ankitsingh/project/dosp/xinu/include/xinu.h', 'r')
        f1 =fx.read()
        if fhead not in f1:
            fy = open('/Users/ankitsingh/project/dosp/xinu/include/xinu.h', 'a+')
            fy.write('\n'+fhead)
            fy.close()
        fx.close()

