import pefile
import sys
import os.path

dll = pefile.PE("pet.dll")
dll_basename = os.path.splitext("pet.dll")[0]

try:
    with open("pet.dll".split("/")[-1].replace(".dll", ".def"), "w") as f:
        f.write("EXPORTS\n")
        for export in dll.DIRECTORY_ENTRY_EXPORT.symbols:
            if export.name:
                f.write('{}={}.{} @{}\n'.format(export.name.decode(), dll_basename, export.name.decode(), export.ordinal))
except:
    print ("failed to create .def file :(")
else:
    print ("successfully create .def file :)")
