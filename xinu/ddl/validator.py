
#Validate an XML ElementTree

device_classes = ("Sensor", "Actuator", "Device")
device_types = ("Physical", "Virtual")
operation_classes = ("Input", "Output")
operation_types = ("Analog", "Digital", "Protocol", "Logical")
number_classes = ("Single", "Multiple")
allowed_status = ("INIT", "BINDED", "ALIGNED", "EXEC", "TERMINATE", "EMERGENCY", "ERROR", "ALL")
reading_type = ("Basic", "Derived", "Physical")
computation_type = ("Aggregate", "Formula", "Map")

def check_computation(elem):
  types = elem.findall("Type")
  assert len(types) == 1, "Must have a computation type"
  assert types[0] in computation_type, "Unknown computation type"
  expressions = elem.findall("Expression")
  assert len(expressions) <= 1, "At most one Expression"

def check_reading(elem):
  types = elem.findall("Type")
  assert len(types) == 1, "Must have one Type"
  assert types[0] in reading_type, "Reading Type unknown"
  measurements = elem.findall("Measurement")
  assert len(measurements) == 1, "Must have one Measurement"
  units = elem.findall("Unit")
  assert len(units) <= 1, "May have at most one Unit"
  ranges = elem.findall("Range")
  assert len(ranges) <= 1, "At most one Range"
  check_range(ranges[0])
  computations = elem.findall("Computation")
  assert len(computations) <= 1, "At most one Computation"
  if computations:
    check_computation(computations[0])

def check_id(elem):
  pass #Not defined

def check_location(elem):
  pass #Not defined

def check_param(elem):
  parameter_names = elem.findall("Parameter_name")
  assert len(parameter_names) == 1, "Parameter name is required"
  parameter_name = parameter_names[0]
  assert parameter_name.text.strip(), "Parameter name cannot be empty"
  chars = elem.findall("Parameter_characterization")
  for char in chars:
    ids = char.findall("id")
    enu = char.findall("enumeration")
    dis = char.findall("discrete")
    assert len(ids) == 1
    assert len(enu) == 1
    assert len(dis) == 1

check_param = lambda x: True

def check_signal(elem):
  operations = elem.findall("Operation")
  assert len(operations) == 1, "Must be exactly one operation mode"
  operation = operations[0]
  assert operation.text in operation_classes, "Signal must be input or output"
  types = elem.findall("Type")
  assert len(types) == 1, "Must be exactly one type"
  type = types[0]
  assert type.text in operation_types, "Signal type unknown"
  measurements = elem.findall("Measurement")
  numbers = elem.findall("Number")
  ranges = elem.findall("Range")
  units = elem.findall("Unit")
  methods = elem.findall("Method_name")
  assert len(measurements) <= 1, "Up to one Measurement"
  assert len(units) <= 1, "Up to one Unit"
  assert len(numbers) <= 1, "Up to one Number"
  assert len(ranges) <= 1, "Up to one Range"
  assert len(methods) <= 1, "Up to one Method_name"
  measurement = measurements[0]
  unit = units[0]
  number = numbers[0]
  range = ranges[0]
  assert number.text in number_classes, "Number class (Single, Multiple) not recognized"
  if range:
    check_range(range)
  if methods:
    method = methods[0]
    assert method.text, "Empty method name"
  
  #Find and check parameters
  for param in elem.findall("Parameter"):
   check_param(param)

  alloweds = elem.findall("Allowed_Status")
  assert len(alloweds) <= 1, "Up to one allowed status listing"
  assert (not alloweds or alloweds[0] in allowed_status), "Unknown Allowed_status"
  
def check_interface(elem):
  signals = elem.findall("Signal")
  readings = elem.findall("Readings")
  assert len(readings) <= 1, "At most one Reading"
  for signal in signals:
    check_signal(signal)
  if readings:
    check_reading(readings[0])

def check_range(elem):
  children = elem.getchildren()
  assert len(children) == 2, "Expected two range endpoints"
  max = elem.find("Max")
  min = elem.find("Min")
  assert max is not None, "Missing Max"
  assert min is not None, "Missing Min"
  assert len(max.getchildren()) == 0, "Max should not have children"
  assert len(min.getchildren()) == 0, "Min should not have children"
  try:
   a = float(max.text)
   b = float(min.text)
   assert a >= b, "Improper Range interval"
  except TypeError as e:
    pass #Shhh...

def check_temperature(elem):
  children = elem.getchildren()
  assert len(children) == 1, "Missing temperature range"
  check_range(children[0])

def check_humidity(elem):
  children = elem.getchildren()
  assert len(children) == 1, "Missing humidity range"
  check_range(children[0])

def check_physical(elem):
  #Dimensions
  assert len(elem.findall("Dimensions")) <= 1, "At most one Dimensions"
  assert len(elem.findall("Operating_environment")) <= 1, "At most one Operating_environment"

  dimensions = elem.find("Dimensions")
  if dimensions is not None:
    check_dimensions(dimensions)

  env = elem.find("Operating_environment")
  if env is not None:
    check_env(env)

def check_dimensions(elem):
  children = elem.getchildren()
  assert len(children) == 3, "Must have three dimensions in Dimensions"
  assert elem.find('Length') is not None, "Missing Length"
  assert elem.find('Width') is not None, "Missing Width"
  assert elem.find('Height') is not None, "Missing Height"

def check_env(elem):
  assert len(elem.findall("Temperature")) <= 1, "Up to one Temperature"
  assert len(elem.findall("Humidity")) <= 1, "Up to one Humidity"
  temperature = elem.find('Temperature')
  if temperature is not None:
    check_temperature(temperature)

  humidity = elem.find('Humidity')
  if humidity is not None:
    check_humidity(humidity)

def validate(tree):
  """Validate a DDL XML tree"""

  #Root element
  root = tree.getroot()
  assert root.tag == "DDL", "Missing main element"
  assert len(root) == 1, "One device per file"
  rchild = root[0]

  #Sensor, Actuator, Complex
  assert rchild.tag in device_classes, "Unknown device class"
  
  #Sensor and Device checks
  if rchild.tag == "Sensor" or rchild.tag == "Device":
    assert len(rchild.findall("Description")) == 1, "One and only one Description"
    assert len(rchild.findall("Interface")) == 1, "One and only one Interface"
    assert len(rchild.getchildren()) == 2, "Sensor or Device incorrect number of child elements"

  #Actuator undefined
  if rchild.tag == "Actuator":
    return

  #Check Description
  description = rchild.find('Description')
  named = False
  typed = False
  vdesc = False
  vendor = False
  version = False
  physical = False
  id = False
  location = False
  for child in description.getchildren():
    if child.tag == "Name":
      assert not named, "Already named"
      assert not child.getchildren(), "Name cannot have a child"
      assert (child.text).strip(), "Name cannot be blank"
      named = True
    elif child.tag == "Device_Type":
      assert not typed, "Already has Device_Type"
      assert child.text in device_types, "Unknown device type"
      assert not child.getchildren(), "Device type cannot have a child"
      typed = True
    elif child.tag == "Verbose_Description":
      assert not vdesc, "Already has a verbose description"
      assert not child.getchildren(), "Verbose description cannot have a child"
      vdesc = True
    elif child.tag == "Vendor":
      assert not vendor, "Already has a Vendor"
      assert not child.getchildren(), "Vendor cannot have a child"
      vendor = True
    elif child.tag == "Version":
      assert not version, "Already has a Version"
      assert not child.getchildren(), "Version cannot have a child"
      version = True
    elif child.tag == "Physical":
      assert not physical, "Already has Physical"

      #Check the physical
      check_physical(child)

      physical = True
    elif child.tag == "UniqueID":
      assert not id, "Already has ID"

      #Check the ID
      check_id(child)

      id = True
    elif child.tag == "Location":
      assert not location, "Already has a Location"

      #Check the Location
      check_location(child)

      location = True
    else:
      assert False, "Unrecognized description tag"

  #Check required tags
  assert named, "Missing Name"
  assert typed, "Missing Device_Type"
  assert physical, "Missing Physical"

  #Check Interface
  interface = rchild.find("Interface")  
  check_interface(interface)
