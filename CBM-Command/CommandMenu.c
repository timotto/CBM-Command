/**************************************************************
Copyright (c) 2010, Payton Byrd
All rights reserved.

Redistribution and use in source and binary forms, with or 
without modification, are permitted provided that the following 
conditions are met:

* Redistributions of source code must retain the above 
  copyright notice, this list of conditions and the following 
  disclaimer.

* Redistributions in binary form must reproduce the above 
  copyright notice, this list of conditions and the following 
  disclaimer in the documentation and/or other materials 
  provided with the distribution.

* Neither the name of Payton Byrd nor the names of its 
  contributors may be used to endorse or promote products 
  derived from this software without specific prior written 
  permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************/

#include <cbm.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CommandMenu.h"
#include "constants.h"
#include "DriveMenu.h"
#include "drives.h"
#include "globals.h"
#include "menus.h"
#include "screen.h"

unsigned isInitialized = FALSE;

char* COMMAND_MENU_LABELS[2];
unsigned char COMMAND_MENU_KEYS[2];

void initCommandMenu(void)
{
	if(!isInitialized)
	{
		COMMAND_MENU_LABELS[0] = COMMAND_MENU_SWAP_PANELS;
		COMMAND_MENU_LABELS[1] = COMMAND_MENU_PANELS_ON_OFF;

		COMMAND_MENU_KEYS[0] = COMMAND_MENU_SWAP_PANELS_KEY;
		COMMAND_MENU_KEYS[1] = COMMAND_MENU_PANELS_ON_OFF_KEY;

		isInitialized = TRUE;
	}
}

void handleCommandMenu(void)
{
	unsigned char key;
	unsigned handleKeys = TRUE;
	unsigned char buffer[39];
	struct panel_drive tempPanel;

	while(handleKeys)
	{
		key = cgetc();
		handleKeys = FALSE;
		switch((int)key)
		{
#ifdef __C128__
		case CH_ESC:
#endif
		case CH_STOP:
			retrieveScreen();
			strcpy(buffer, "Escaping menu...");
			writeStatusBar(buffer, 0, 20);
			return;
			break;

		case CH_CURS_UP:
			if(currentMenuLine == 0) currentMenuLine = COMMAND_MENU_COUNT - 1;
			else currentMenuLine--;
			retrieveScreen();
			drawCommandMenu(currentMenuX);
			handleKeys = TRUE;
			break;

		case CH_CURS_DOWN:
			if(currentMenuLine == COMMAND_MENU_COUNT - 1) currentMenuLine = 0;
			else currentMenuLine++;
			retrieveScreen();
			drawCommandMenu(currentMenuX);
			handleKeys = TRUE;
			break;

		case CH_CURS_LEFT:
			retrieveScreen();
			writeMenu(options);
			return;
			break;

		case CH_CURS_RIGHT:
			retrieveScreen();
			writeMenu(right);
			return;
			break;

		case CH_ENTER:
			key = COMMAND_MENU_KEYS[currentMenuLine];

		default:
			if(key == COMMAND_MENU_SWAP_PANELS_KEY)
			{
				tempPanel.drive = leftPanelDrive.drive;
				tempPanel.head = leftPanelDrive.head;
				tempPanel.length = leftPanelDrive.length;
				tempPanel.tail = leftPanelDrive.tail;
				tempPanel.currentIndex = leftPanelDrive.currentIndex;
				tempPanel.displayStartAt = leftPanelDrive.displayStartAt;

				leftPanelDrive.drive = rightPanelDrive.drive;
				leftPanelDrive.head = rightPanelDrive.head;
				leftPanelDrive.length = rightPanelDrive.length;
				leftPanelDrive.tail = rightPanelDrive.tail;
				leftPanelDrive.currentIndex = rightPanelDrive.currentIndex;
				leftPanelDrive.displayStartAt = rightPanelDrive.displayStartAt;

				rightPanelDrive.drive = tempPanel.drive;
				rightPanelDrive.head = tempPanel.head;
				rightPanelDrive.length = tempPanel.length;
				rightPanelDrive.tail = tempPanel.tail;
				rightPanelDrive.currentIndex = tempPanel.currentIndex;
				rightPanelDrive.displayStartAt = tempPanel.displayStartAt;
				
				setupScreen();
				writeMenuBar();
				displayPanels();
				arePanelsOn = TRUE;
			}
			else if(key == COMMAND_MENU_PANELS_ON_OFF_KEY)
			{
				if(arePanelsOn)
				{
					setupScreen();
					writeMenuBar();
					arePanelsOn = FALSE;
				}
				else
				{
					retrieveScreen();
					displayPanels();
					arePanelsOn = TRUE;
				}
			}
			else
			{
				handleKeys = TRUE;
			}
			break;
		}
	}
}

void displayPanels(void)
{
	if(leftPanelDrive.drive != NULL)
	{
		rereadDrivePanel(left);
	}
	if(rightPanelDrive.drive != NULL)
	{
		rereadDrivePanel(right);
	}
}