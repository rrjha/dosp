
#Validate an XML ElementTree

device_classes = ("Sensor", "Actuator", "Device")
device_types = ("Physical", "Virtual")

def check_physical(elem):
  pass #TODO

def check_location(elem):
  pass #TODO

def check_id(elem):
  pass #TODO

def validate(tree):
  """Validate a DDL XML tree"""

  #Root element
  root = tree.getroot()
  assert root.tag == "DDL", "Missing main element"
  assert len(root) == 1, "One device per file"
  child = root[0]

  #Sensor, Actuator, Complex
  assert child.tag in device_classes, "Unknown device class"
  
  #Sensor and Device checks
  if child.tag == "Sensor" or child.tag == "Device":
    assert len(child.findall("Description")) == 1, "One and only one Description"
    assert len(child.findall("Interface")) == 1, "One and only one Interface"
    assert len(child.getchildren()) == 2, "Sensor or Device incorrect number of child elements"

  #Actuator undefined
  if child.tag == "Actuator":
    return

  #Check Description
  description = child.find('Description')
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
