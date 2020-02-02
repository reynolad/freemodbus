/*
 * FreeModbus Libary: MSP430 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Static variables ---------------------------------*/
UCHAR           ucCriticalNesting = 0x00;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    ENTER_CRITICAL_SECTION(  );
    MB_enableSerial(xRxEnable, xTxEnable);
    EXIT_CRITICAL_SECTION(  );
}

BOOL
xMBPortSerialInit( UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL            bInitialized = TRUE;

    switch ( eParity )
    {
    case MB_PAR_NONE:
    case MB_PAR_ODD:
    case MB_PAR_EVEN:
        break;
    default:
        bInitialized = FALSE;
        break;
    }
    switch ( ucDataBits )
    {
    case 8:
    case 7:
        break;
    default:
        bInitialized = FALSE;
    }
    if( bInitialized )
    {
        ENTER_CRITICAL_SECTION(  );

        MB_initSerial(ulBaudRate, ucDataBits, eParity);

        EXIT_CRITICAL_SECTION(  );
    }
    return bInitialized;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    MB_putByteSerial(ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    MB_getByteSerial((uint16_t *)pucByte);
    return TRUE;
}
void MB_rxISR(void)
{
    pxMBFrameCBByteReceived(  );
}

void MB_txISR(void)
{
    pxMBFrameCBTransmitterEmpty(  );
}

void EnterCriticalSection( void )
{
    if( ucCriticalNesting == 0 )
    {
        MB_enterCriticalSection();
    }
    ucCriticalNesting++;
}

void ExitCriticalSection( void )
{
    ucCriticalNesting--;
    if( ucCriticalNesting == 0 )
    {
        MB_exitCriticalSection();
    }
}
