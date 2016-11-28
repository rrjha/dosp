all: do_led do_temp do_accel

do_temp: tmp36.xml
		python3 parser.py tmp36.xml

do_led: led.xml
		python3 parser.py led.xml

do_accel: adxl345.xml
		python3 parser.py adxl345.xml

