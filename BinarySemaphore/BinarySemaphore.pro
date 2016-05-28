TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.c

HEADERS += \
    Core/Array.h \
    Core/Bool.h \
    Core/Borland.h \
    Core/Console.h \
    Core/Core.h \
    Core/CoreData.h \
    Core/CoreFunctions.h \
    MMT/MemoryMapTable.h \
    MMT/MemoryMapTableCore.h \
    MMT/MemoryMapTableNode.h \
    MMT/MemoryMapTableRow.h \
    PCB/ProcessControlBlockCore.h \
    PCB/ProcessControlBlockNode.h \
    PCB/ProcessControlBlockRow.h \
    PCB/ProcessControlBlockTable.h \
    QUEUE/Queue.h \
    QUEUE/QueueNode.h \
    QUEUE/QueueRow.h \
    BinarySemaphore.h \
    CPUManager.h \
    ProgramSettings.h
