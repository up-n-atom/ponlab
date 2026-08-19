from ctypes import*

# give location of dll
mydll = windll.LoadLibrary("c:/work/mmaz/dut/output/x64/Debug/dut_c_wrapper.dll")

mydll.DUT_getVersion.restype = c_char_p
version = mydll.DUT_getVersion()
mydll.DUT_getSharedHeaderTag.restype = c_char_p
sharedHeaderTag = mydll.DUT_getSharedHeaderTag()
print(f'Version: {version.decode()} ({sharedHeaderTag.decode()})')

mydll.Connection_Create.restype = c_void_p
connection = mydll.Connection_Create()

mydll.Connection_open.argtypes = [c_void_p, c_char_p, c_uint16, c_uint32]
mydll.Connection_open.restype = c_bool
ok = mydll.Connection_open(connection, b"192.168.1.1", 22222, 3000)

mydll.Logger_CreateWindowsConsoleLogger.argtypes = [c_bool, c_uint32]
mydll.Logger_CreateWindowsConsoleLogger.restype = c_void_p
logger = mydll.Logger_CreateWindowsConsoleLogger(True, 5)

mydll.DUT_Create.argtypes = [c_uint8, c_void_p, c_void_p]
mydll.DUT_Create.restype = c_void_p
dut = mydll.DUT_Create(0, connection, logger)

mydll.DUT_driverInit.argtypes = [c_void_p, c_bool, c_uint32, c_uint32]
mydll.DUT_driverInit.restype = c_bool
ok = mydll.DUT_driverInit(dut, False, 2, 1024)

mydll.DUT_driverRelease.argtypes = [c_void_p]
mydll.DUT_driverRelease.restype = c_bool
ok = mydll.DUT_driverRelease(dut)

mydll.Connection_close.argtypes = [c_void_p]
mydll.Connection_close.restype = None
mydll.Connection_close(connection)

mydll.DUT_Destroy.argtypes = [POINTER(c_void_p)]
mydll.DUT_Destroy.restype = None
p = c_void_p(dut)
mydll.DUT_Destroy(byref(p))

mydll.Logger_Destroy.argtypes = [POINTER(c_void_p)]
mydll.Logger_Destroy.restype = None
p = c_void_p(logger)
mydll.Logger_Destroy(byref(p))

mydll.Connection_Destroy.argtypes = [POINTER(c_void_p)]
mydll.Connection_Destroy.restype = None
p = c_void_p(connection)
mydll.Connection_Destroy(byref(p))

print('Done!')