//
//  main.c
//  IOKitUSBAccessTest
//
//  Created by Zeeshan Hooda on 5/12/20.
//  Copyright © 2020 Deceptive Labs. All rights reserved.
//

#include <stdio.h>

// APPLE INCLUDES
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/USBSpec.h>

// MARK: Main function
int main(int argc, const char * argv[]) {
    
    // MARK: Declarations
    CFMutableDictionaryRef matchingDictionary = NULL;
    SInt32 idVendor = 0x000; // set vendor id
    SInt32 idProduct = 0x000; // set product id
    io_iterator_t iterator = 0;
    io_service_t usbRef;
    SInt32 score;
    IOCFPlugInInterface** plugin;
    IOUSBDeviceInterface300** usbDevice = NULL;
    IOReturn ret;
    IOUSBConfigurationDescriptorPtr config;
    IOUSBFindInterfaceRequest interfaceRequest;
    IOUSBInterfaceInterface300** usbInterface;
    char out[] = { 0x00, 0x00 }; // set data to send
    char* in;
    UInt32 numBytes;
    
    // MARK: Attempt to find device using vendor and product id
    matchingDictionary = IOServiceMatching(kIOUSBDeviceClassName);
    CFDictionaryAddValue(matchingDictionary, CFSTR(kUSBVendorID), CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &idVendor));
    CFDictionaryAddValue(matchingDictionary, CFSTR(kUSBProductID), CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &idProduct));
    IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDictionary, &iterator);
    usbRef = IOIteratorNext(iterator);
    if (usbRef == 0) {
        printf("Device not found\n");
        return -1;
    }
    IOObjectRelease(iterator);
    IOCreatePlugInInterfaceForService(usbRef, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugin, &score);
    IOObjectRelease(usbRef);
    (*plugin)->QueryInterface(plugin, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID300), (LPVOID)&usbDevice);
    (*plugin)->Release(plugin);
    
    // MARK: Device found, now open and set config as active?
    ret = (*usbDevice)->USBDeviceOpen(usbDevice);
    if (ret == kIOReturnSuccess) {
        // set first configuration as active
        ret = (*usbDevice)->GetConfigurationDescriptorPtr(usbDevice, 0, &config);
        if (ret != kIOReturnSuccess) {
            printf("Could not set active configuration )error: %x\n", ret);
            return -1;
        }
        (*usbDevice)->SetConfiguration(usbDevice, config->bConfigurationValue);
    } else if (ret == kIOReturnExclusiveAccess) {
        //this is not a problem we can still do some things
    } else {
        printf("Could not open device (error: %x)\n", ret);
    }
    
    // MARK: I'm not sure what this does now
    interfaceRequest.bInterfaceClass = kIOUSBFindInterfaceDontCare;
    interfaceRequest.bInterfaceSubClass = kIOUSBFindInterfaceDontCare;
    interfaceRequest.bInterfaceProtocol = kIOUSBFindInterfaceDontCare;
    interfaceRequest.bAlternateSetting = kIOUSBFindInterfaceDontCare;
    (*usbDevice)->CreateInterfaceIterator(usbDevice, &interfaceRequest, &iterator);
    IOIteratorNext(iterator); // skip interface #0
    usbRef = IOIteratorNext(iterator);
    IOObjectRelease(iterator);
    IOCreatePlugInInterfaceForService(usbRef, kIOUSBInterfaceUserClientTypeID, kIOCFPlugInInterfaceID, &plugin, &score);
    IOObjectRelease(usbRef);
    (*plugin)->QueryInterface(plugin, CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID300), (LPVOID)&usbInterface);
    (*plugin)->Release(plugin);
    
    // MARK: Found interface, let's use it
    ret = (*usbInterface)->USBInterfaceOpen(usbInterface);
    if (ret != kIOReturnSuccess) {
        printf("Could not open interface (error: %x)\n", ret);
        return -1;
    }
    
    // MARK: Sent and recieve data
    
    // Send data through pipe 1
    (*usbInterface)->WritePipe(usbInterface, 1, out, sizeof(out));
    
    // Read data through pipe 2
    numBytes = 0x40; // 64 bytes
    in = malloc(numBytes);
    ret = (*usbInterface)->ReadPipe(usbInterface, 2, in, &numBytes);
    if (ret == kIOReturnSuccess) {
        printf("Read %d bytes\n", numBytes);
    } else {
        printf("Read failed (error: %x)\n", ret);
    }
    
    (*usbInterface)->USBInterfaceClose(usbInterface);
    (*usbDevice)->USBDeviceClose(usbDevice);
    
    return 0;
}
