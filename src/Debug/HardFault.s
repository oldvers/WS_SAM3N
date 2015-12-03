
                AREA    HARD_FAULT, CODE, READONLY

                EXPORT  HardFault_Handler
                IMPORT  PrintFaultInfo
HardFault_Handler\
                PROC
                TST     LR, #4
                ITE     EQ
                MRSEQ   R0, MSP
                MRSNE   R0, PSP
                B       PrintFaultInfo
                ENDP

                END
