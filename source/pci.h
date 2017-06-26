#include "stdlib.h"

#define PCI_CONFIG_PORT      0x0CF8
#define PCI_DATA_PORT        0x0CFC

#define PCI_MAX_BUSES        255
#define PCI_MAX_DEVICES      32
#define PCI_MAX_FUNCTIONS    8

#define PCI_HEADERTYPE_NORMAL        0
#define PCI_HEADERTYPE_BRIDGE        1
#define PCI_HEADERTYPE_CARDBUS       2
#define PCI_HEADERTYPE_MULTIFUNC     0x80

typedef union {
	struct {
		uint16_t vendorID;
		uint16_t deviceID;
		uint16_t commandReg;
		uint16_t statusReg;
		uint8_t revisionID;
		uint8_t progIF;
		uint8_t subClassCode;
		uint8_t classCode;
		uint8_t cachelineSize;
		uint8_t latency;
		uint8_t headerType;
		uint8_t BIST;
	} __attribute__((packed)) option;
	uint32_t header[4];
} __attribute__((packed)) PCIDevHeader;

void ReadConfig32(uint32_t bus, uint32_t dev, uint32_t func, uint32_t reg, uint32_t *data);
char *GetPCIDevClassName(uint32_t class_code);
void PCIScan();
