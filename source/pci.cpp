#include "pci.h"

typedef uint16_t ioport_t;

typedef struct {
	uint32_t class_code;
	char name[32];
} PCIClassName;

static PCIClassName g_PCIClassNames[] = {
	{ 0x00, "before PCI 2.0"},
	{ 0x01, "disk controller"},
	{ 0x02, "network interface"},
	{ 0x03, "graphics adapter"},
	{ 0x04, "multimedia controller"},
	{ 0x05, "memory controller"},
	{ 0x06, "bridge device"},
	{ 0x07, "communication controller"},
	{ 0x08, "system device"},
	{ 0x09, "input device"},
	{ 0x0a, "docking station"},
	{ 0x0b, "CPU"},
	{ 0x0c, "serial bus"},
	{ 0x0d, "wireless controller"},
	{ 0x0e, "intelligent I/O controller"},
	{ 0x0f, "satellite controller"},
	{ 0x10, "encryption controller"},
	{ 0x11, "signal processing controller"},
	{ 0xFF, "proprietary device"}
};

typedef union {
	struct {
		uint32_t zero : 2;
		uint32_t reg_num : 6;
		uint32_t func_num : 3;
		uint32_t dev_num : 5;
		uint32_t bus_num : 8;
		uint32_t reserved : 7;
		uint32_t enable_bit : 1;
	};
	uint32_t val;
} PCIConfigAddres;

static inline void in32 (ioport_t port, uint32_t *data) {
	asm volatile ("inl %%dx, %%eax" : "=a" (*data) : "d" (port));
}

static inline void out32 (ioport_t port, uint32_t data) {
	asm volatile ("outl %%eax, %%dx" : : "a" (data), "d" (port));
}

void ReadConfig32(uint32_t bus, uint32_t dev, uint32_t func, uint32_t reg, uint32_t *data) {
	PCIConfigAddres addr;

	addr.val = 0;
	addr.enable_bit = 1;
	addr.reg_num =  reg;
	addr.func_num = func;
	addr.dev_num =  dev;
	addr.bus_num =  bus;

	out32(PCI_CONFIG_PORT, addr.val);
	in32(PCI_DATA_PORT, data);
	return;
}

char *GetPCIDevClassName(uint32_t class_code) {
	int i;
	for (int i = 0; i < sizeof(g_PCIClassNames)/sizeof(g_PCIClassNames[0]); i++) {
		if (g_PCIClassNames[i].class_code == class_code){
			return g_PCIClassNames[i].name;
		}
	}
	return NULL;
}

int ReadPCIDevHeader(uint32_t bus, uint32_t dev, uint32_t func, PCIDevHeader *p_pciDevice) {
	if (p_pciDevice == 0) { return 1; }

	for (int i = 0; i < sizeof(p_pciDevice->header)/sizeof(p_pciDevice->header[0]); i++){
		ReadConfig32(bus, dev, func, i, &p_pciDevice->header[i]);
	}

	if (p_pciDevice->option.vendorID == 0x0000 || p_pciDevice->option.vendorID == 0xffff || p_pciDevice->option.deviceID == 0xffff) {
		return 1;
	}

	return 0;
}

void PrintPCIDevHeader(uint32_t bus, uint32_t dev, uint32_t func, PCIDevHeader *p_pciDevice) {
	printf("bus=0x%x dev=0x%x func=0x%x venID=0x%x devID=0x%x", bus, dev, func, p_pciDevice->option.vendorID, p_pciDevice->option.deviceID);

	char *class_name = GetPCIDevClassName(p_pciDevice->option.classCode);
	if (class_name) { printf(" class_name=%s", class_name); }

	printf("\n");
}

void PCIScan(void) {
	int bus;
	int dev;

	for (bus = 0; bus < PCI_MAX_BUSES; bus++) {
		for (dev = 0; dev < PCI_MAX_DEVICES; dev++) {
			uint32_t func = 0;
			PCIDevHeader pci_device;

			if (ReadPCIDevHeader(bus, dev, func, &pci_device)) { continue; }

			PrintPCIDevHeader(bus, dev, func, &pci_device);

			if (pci_device.option.headerType & PCI_HEADERTYPE_MULTIFUNC) {
				for (func = 1; func < PCI_MAX_FUNCTIONS; func++) {
					if (ReadPCIDevHeader(bus, dev, func, &pci_device)) { continue; }
					PrintPCIDevHeader(bus, dev, func, &pci_device);
				}
			}
		}
	}
}
