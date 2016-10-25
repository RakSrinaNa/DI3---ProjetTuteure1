/*
 * Copyright 2010 Sébastien Aupetit <sebastien.aupetit@univ-tours.fr>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: CustomerRecord.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CustomerRecord.h>

void IMPLEMENT(CustomerRecord_setValue_name)(CustomerRecord * record, const char * value)
{
	unsigned int i = 0;
	while(i < CUSTOMERRECORD_NAME_SIZE - 1 && value[i] != '\0') /* For each character while we didn't went over the size */
	{
		record->name[i] = value[i];
		i++;
	}
	record->name[i] = '\0';
}

void IMPLEMENT(CustomerRecord_setValue_address)(CustomerRecord * record, const char * value)
{
	unsigned int i = 0;
	while(i < CUSTOMERRECORD_ADDRESS_SIZE - 1 && value[i] != '\0') /* For each character while we didn't went over the size */
	{
		record->address[i] = value[i];
		i++;
	}
	record->address[i] = '\0';
}

void IMPLEMENT(CustomerRecord_setValue_postalCode)(CustomerRecord * record, const char * value)
{
	unsigned int i = 0;
	while(i < CUSTOMERRECORD_POSTALCODE_SIZE - 1 && value[i] != '\0') /* For each character while we didn't went over the size */
	{
	   record->postalCode[i] = value[i];
	   i++;
	}
	record->postalCode[i] = '\0';
}

void IMPLEMENT(CustomerRecord_setValue_town)(CustomerRecord * record, const char * value)
{
	unsigned int i = 0;
	while(i < CUSTOMERRECORD_TOWN_SIZE - 1 && value[i] != '\0') /* For each character while we didn't went over the size */
	{
	   record->town[i] = value[i];
	   i++;
	}
	record->town[i] = '\0';
}

char * IMPLEMENT(CustomerRecord_getValue_name)(CustomerRecord * record)
{
    return duplicateString(record->name);
}

char * IMPLEMENT(CustomerRecord_getValue_address)(CustomerRecord * record)
{
    return duplicateString(record->address);
}

char * IMPLEMENT(CustomerRecord_getValue_postalCode)(CustomerRecord * record)
{
    return duplicateString(record->postalCode);
}

char * IMPLEMENT(CustomerRecord_getValue_town)(CustomerRecord * record)
{
    return duplicateString(record->town);
}

void IMPLEMENT(CustomerRecord_init)(CustomerRecord * record)
{
    unsigned int i;
    for(i = 0; i < CUSTOMERRECORD_NAME_SIZE; i++)
    {
        record->name[i] = '\0';
    }
    for(i = 0; i < CUSTOMERRECORD_ADDRESS_SIZE; i++)
    {
        record->address[i] = '\0';
    }
    for(i = 0; i < CUSTOMERRECORD_POSTALCODE_SIZE; i++)
    {
        record->postalCode[i] = '\0';
    }
    for(i = 0; i < CUSTOMERRECORD_TOWN_SIZE; i++)
    {
        record->town[i] = '\0';
    }
}

void IMPLEMENT(CustomerRecord_finalize)(CustomerRecord * record)
{
	record = record;
}

void IMPLEMENT(CustomerRecord_read)(CustomerRecord * record, FILE * file)
{
  provided_CustomerRecord_read(record, file);
}

void IMPLEMENT(CustomerRecord_write)(CustomerRecord * record, FILE * file)
{
  provided_CustomerRecord_write(record, file);
}
