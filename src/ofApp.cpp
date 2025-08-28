#include "ofApp.h"
#include "JoyShockLibrary.h"

ofApp *joyOSCMapperPointer;
void getJslCallback(int joyconId, JOY_SHOCK_STATE lastButtonsStickData, JOY_SHOCK_STATE newButtonsStickData, IMU_STATE lastRawIMUData, IMU_STATE newRawIMUData, float secsBetweenReports) {
	joyOSCMapperPointer->updateJoyconData(joyconId, newButtonsStickData, newRawIMUData);
}

void ofApp::updateJoyconData(int joyconId, JOY_SHOCK_STATE newButtonsStickData, IMU_STATE newRawIMUData) {
	int firstPos = (numDevicesConnectedSum - numDevicesConnected);
	int joyconPosVec = joyconId - firstPos;
	if (joyconsVec[joyconPosVec].GUIToggle)
		joyconsVec[joyconPosVec].updateData(newButtonsStickData, newRawIMUData, useRawIMUData, useCookedIMUData);
}

//--------------------------------------------------------------
void ofApp::setup(){
	joyOSCMapperPointer = this;//_n1
	font.load(OF_TTF_MONO, 10);

	int screenWidth = ofGetScreenWidth();
	int screenHeight = ofGetScreenHeight();
	int winWidthProportion = 5;
	int winHeightProportion = 3;
	int hMargin = screenWidth / winWidthProportion;
	int vMargin = screenHeight / winHeightProportion;
	winWidth = screenWidth - (2 * hMargin);
	winHeight = screenHeight - (2 * vMargin);

	ofSetWindowShape(winWidth, winHeight);
	ofSetWindowPosition(hMargin, vMargin);
	ofSetWindowTitle("joyOSCMapper");
	ofBackground(10);
	ofSetFrameRate(67);
	ofSetVerticalSync(false);

	setupGuiControl();
	instantiateConnectedJoycons();
	setupGuiJoyconsList();

	guiShortcuts.setup();
	guiShortcuts.setName("Shortcuts/Help");
	guiShortcuts.add(sShortcut.setup("s-show/hide Shortcuts    ", sShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(gShortcut.setup("g-show/hide GUIControl   ", gShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(uShortcut.setup("  u-updateConnected      ", uShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(dShortcut.setup("  d-disconnectDisposeAll ", dShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(jShortcut.setup("j-show/hide JoyconsList  ", jShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(vShortcut.setup("v-un/toggle VirtualJoys  ", vShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(aShortcut.setup("  a-addAVirualJoycon     ", aShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(rShortcut.setup("  r-removeAVirualJoycon  ", rShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(cShortcut.setup("c-calibrateJoycons       ", cShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(oShortcut.setup("o-un/toggle oscOnly mode ", oShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(xShortcut.setup("x-show/hide GUIGraphConf ", xShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(qShortcut.setup("  q-useRawIMUData        ", qShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(wShortcut.setup("  w-useCookedIMUData     ", wShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(eShortcut.setup("  e-useEulerOrientation  ", eShortcut, guiWidth, guiLineHeight));
	guiShortcuts.add(leftClickHelp.setup("lClick-test vrtJoy input ", leftClickHelp, guiWidth, guiLineHeight));
	guiShortcuts.add(rightClickHelp.setup("rClick-check oscAddress ", rightClickHelp, guiWidth, guiLineHeight));
	guiShortcuts.setBackgroundColor(guiColor);
	sShortcut.setBackgroundColor(guiColor);
	dShortcut.setBackgroundColor(guiColor);
	gShortcut.setBackgroundColor(guiColor);
	jShortcut.setBackgroundColor(guiColor);
	uShortcut.setBackgroundColor(guiColor);
	vShortcut.setBackgroundColor(guiColor);
	aShortcut.setBackgroundColor(guiColor);
	rShortcut.setBackgroundColor(guiColor);
	cShortcut.setBackgroundColor(guiColor);
	oShortcut.setBackgroundColor(guiColor);
	xShortcut.setBackgroundColor(guiColor);
	qShortcut.setBackgroundColor(guiColor);
	wShortcut.setBackgroundColor(guiColor);
	eShortcut.setBackgroundColor(guiColor);
	leftClickHelp.setBackgroundColor(guiColor);
	rightClickHelp.setBackgroundColor(guiColor);

	guiGraphHelp.setup();
	guiGraphHelp.setName("GraphColors");
	guiGraphHelp.add(wColorHelp.setup("  w-quaternion           ", vShortcut, guiWidth, guiLineHeight));
	guiGraphHelp.add(xiColorHelp.setup("  x/i-quaternion         ", vShortcut, guiWidth, guiLineHeight));
	guiGraphHelp.add(yjColorHelp.setup("  y/j-quaternion         ", vShortcut, guiWidth, guiLineHeight));
	guiGraphHelp.add(zkColorHelp.setup("  z/k-quaternion         ", vShortcut, guiWidth, guiLineHeight));
	guiGraphHelp.setBackgroundColor(guiColor);
	wColorHelp.setBackgroundColor(ofColor(200, 200, 200, 150));
	xiColorHelp.setBackgroundColor(ofColor(255, 100, 100, 150));
	yjColorHelp.setBackgroundColor(ofColor(0, 255, 0, 150));
	zkColorHelp.setBackgroundColor(ofColor(50, 100, 255, 150));

	guiConfigVirtualJoycons.setup();
	guiConfigVirtualJoycons.setName("ConfigVirtualJoycons");
	guiConfigVirtualJoycons.add(addVirtualJoycon.setup("addVirtualJoycon", guiWidth, guiLineHeight));
	guiConfigVirtualJoycons.add(removeAVirtualJoycon.setup("removeAVirtualJoycon", guiWidth, guiLineHeight));
	guiConfigVirtualJoycons.add(removeAllVirtual.setup("removeAllVirtual", guiWidth, guiLineHeight));
	guiConfigVirtualJoycons.add(virtualJoyconsAddedLabel.setup("numVirtualJoycons", virtualJoyconsAddedLabel, guiWidth, guiLineHeight));
	virtualJoyconsAddedLabel.operator=(ofToString(numVirtualJoycons));
	guiConfigVirtualJoycons.setBackgroundColor(guiColor);
	addVirtualJoycon.setBackgroundColor(guiColor);
	removeAVirtualJoycon.setBackgroundColor(guiColor);
	removeAllVirtual.setBackgroundColor(guiColor);
	virtualJoyconsAddedLabel.setBackgroundColor(guiColor);
	guiConfigVirtualJoycons.setPosition(guiControl.getPosition().x - guiWidth - BORDER, BORDER);

	guiGraphConfig.setup();
	guiGraphConfig.setName("GUIGraphConfig");
	guiGraphConfig.add(imuVectorSizeSlider.setup("imuVectorSize", DEFAULT_IMUVECTORSSIZE, int(DEFAULT_IMUVECTORSSIZE/2), DEFAULT_IMUVECTORSSIZE * 10, guiWidth, guiLineHeight));
	guiGraphConfig.add(useRawIMUData.setup("useRawIMUData", true, guiWidth, guiLineHeight));
	guiGraphConfig.add(useCookedIMUData.setup("useCookedIMUData", true, guiWidth, guiLineHeight));
	guiGraphConfig.add(useEulerOrientation.setup("useEulerOrientation", false, guiWidth, guiLineHeight));
	guiGraphConfig.setBackgroundColor(guiColor);
	imuVectorSizeSlider.setBackgroundColor(guiColor);
	useEulerOrientation.setBackgroundColor(guiColor);
	useRawIMUData.setBackgroundColor(guiColor);
	useCookedIMUData.setBackgroundColor(guiColor);
	guiGraphConfig.setPosition(winWidth - guiWidth - BORDER, winHeight - (5 * guiLineHeight) - BORDER);

	if (numDevicesConnected == 0) {
		showShortcutsHelp = true;
		showGUIGraphConfig = false;
	}

	winWidthProportion = 4;
	winHeightProportion = 3;
	hMargin = screenWidth / winWidthProportion;
	vMargin = screenHeight / winHeightProportion;

	generalConfigWindowWidth = screenWidth - hMargin;
	generalConfigWindowHeight = screenHeight - vMargin;
	generalConfigWindowSettings.setSize(generalConfigWindowWidth, generalConfigWindowHeight);
	generalConfigWindowSettings.setPosition(ofVec2f(hMargin / 2, vMargin / 2));
	generalConfigWindowSettings.resizable = false;

	joyconConfigWindowSettings.setSize(screenWidth - hMargin, screenHeight - vMargin);
	joyconConfigWindowSettings.setPosition(ofVec2f(hMargin / 2, vMargin / 2));

	oscOnlyInfo << "The oscOnly mode is on. All joycon drawings have stoped," << '\n' << " but their respective OSC messages are still being sent." << endl;
	oscReceiver.setup(oscReceiverPort);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle("joyOSCMapper fps: " + ofToString(ofGetFrameRate()));

	checkAllButtonStates();

	if (joyconsVec.size() > 0) {
		int selectedJoyconsCount = 0;
		for (int index = 0; index < joyconsVec.size(); index++) {
			if (joyconsVec[index].GUIToggle && (!joyconsVec[index].isVirtual || (joyconsVec[index].isVirtual && useVirtualJoycons))) {
				selectedJoyconsCount++;
				joyconsVec[index].updateGraphsValues(useRawIMUData, useCookedIMUData);
				if (joyconsVec[index].isVirtual)
					joyconsVec[index].sendNewInputsAsOSC(joyconsVec[index].currentInputValues);
			}
		}

		if (selectedJoyconsCount != numSelectedJoycons && !oscOnly) {
			numSelectedJoycons = selectedJoyconsCount;
			updateJoyconsDrawings();
		}
	}

	if (oscReceiver.hasWaitingMessages()) {
		ofxOscMessage waitingMsg;
		oscReceiver.getNextMessage(waitingMsg);
		if (waitingMsg.getAddress() == DEFAULT_OSCONLY_OSCADDRESS) {
			oscOnly = waitingMsg.getArgAsInt(0);
		}
		else if (waitingMsg.getAddress() == DEFAULT_GETJOYCONTYPEANDCOLOR_OSCADDRESS) {
			int joyconIndexOnVec = waitingMsg.getArgAsInt(0);
			if (joyconIndexOnVec < joyconsVec.size()) {
				joyconsVec[joyconIndexOnVec].sendJoyconTypeAndColorAsOsc();
			}
			else {
				cout << "Index for getJoyconTypeAndColor out of joyconsVec size" << endl;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (!oscOnly) {
		for (auto& joycon : joyconsVec) {
			if (joycon.GUIToggle && (!joycon.isVirtual || (joycon.isVirtual && useVirtualJoycons))) {
				joycon.drawJoycon(useRawIMUData, useCookedIMUData);
			}
		}
	}
	else {
		font.drawString(oscOnlyInfo.str(), (winWidth - font.stringWidth(oscOnlyInfo.str()))/2, (winHeight - font.stringHeight(oscOnlyInfo.str())) / 2);
	}

	if (showGuiControl)
		guiControl.draw();

	if (showGuiJoyconsList)
		guiJoyconsList.draw(); 

	if (showGUIGraphConfig)
		guiGraphConfig.draw();

	if (showShortcutsHelp && showGuiControl) {
		guiShortcuts.draw();
		guiGraphHelp.draw();
	}

	if (useVirtualJoycons && showGuiControl)
		guiConfigVirtualJoycons.draw();

	if (clickedButtonOscMessage != "") {
		ofDrawBitmapString(clickedButtonOscMessage, clickedButtonPos.x, clickedButtonPos.y);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 'a':
			if (useVirtualJoycons) {
				executeAddVirtualJoycon = true;
				if (joyconsVec.size() == 0)
					showShortcutsHelp = false;
			}
		break;
		case 'c':
			calibrateJoycons = !calibrateJoycons;
			break;
		case 'd':
			executeDisconnectAndDispose = true;
			break;
		case 'o':
			oscOnly = !oscOnly;
			break;
		case 'g':
			showGuiControl = !showGuiControl;
			break;
		case 'j':
			showGuiJoyconsList = !showGuiJoyconsList;
			break;
		case 'l':
			showGuiJoyconsList = !showGuiJoyconsList;
			break;
		case 'h':
			showShortcutsHelp = !showShortcutsHelp;
			break;
		case 's':
			showShortcutsHelp = !showShortcutsHelp;
			break;
		case 'v':
			useVirtualJoycons = !useVirtualJoycons;
			break;
		case 'r':
			executeRemoveAVirtualJoycon = true;
			break;
		case 'x':
			showGUIGraphConfig = !showGUIGraphConfig;
			break;
		case 'q':
			useRawIMUData = !useRawIMUData;
			break;
		case 'w':
			useCookedIMUData = !useCookedIMUData;
			if (useCookedIMUData == false) {
				useEulerOrientation = false;
			}
			break;
		case 'e':
			useEulerOrientation = !useEulerOrientation;
			if (useEulerOrientation == true) {
				useCookedIMUData = true;
			}
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	for (int index = 0; index < joyconsVec.size(); index++) {
		if (x >= joyconsVec[index].celPosX && x <= joyconsVec[index].celPosX + joyconsVec[index].celWidth &&
			y >= joyconsVec[index].celPosY && y <= joyconsVec[index].celPosY + joyconsVec[index].celHeight) {
			joyconCelPressedIndex = index;
			break;
		}
	}

	if (joyconCelPressedIndex >= 0) {
		if (button == OF_MOUSE_BUTTON_LEFT && joyconsVec[joyconCelPressedIndex].isVirtual) {
			clickedButtonOscTag = joyconsVec[joyconCelPressedIndex].checkMouseClick(x, y, button, useRawIMUData, useCookedIMUData);
			if (joyconsVec[joyconCelPressedIndex].clickedInputPointer != NULL) {
				joyconsVec[joyconCelPressedIndex].oscSender.sendMessage(
					joyconsVec[joyconCelPressedIndex].getInputOscMessage(
						clickedButtonOscTag,
						*joyconsVec[joyconCelPressedIndex].clickedInputPointer
					)
				);
			}
		}
		else{
			clickedButtonOscMessage = joyconsVec[joyconCelPressedIndex].checkMouseClick(x, y, button, useRawIMUData, useCookedIMUData);
			int messageWidth = font.stringWidth(clickedButtonOscMessage);
			if (x + messageWidth < winWidth)
				clickedButtonPos.x = x;
			else
				clickedButtonPos.x = x - ((x + messageWidth) - winWidth);

			int messageHeight = font.stringHeight(clickedButtonOscMessage);
			if (y + messageHeight < winHeight)
				clickedButtonPos.y = y;
			else
				clickedButtonPos.y = y - ((y + messageHeight) - winHeight);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (joyconCelPressedIndex >= 0) {
		if (button == OF_MOUSE_BUTTON_LEFT && joyconsVec[joyconCelPressedIndex].clickedInputPointer != NULL) {
			*joyconsVec[joyconCelPressedIndex].clickedInputPointer = false; 
			
			joyconsVec[joyconCelPressedIndex].oscSender.sendMessage(
				joyconsVec[joyconCelPressedIndex].getInputOscMessage(
					clickedButtonOscTag,
					*joyconsVec[joyconCelPressedIndex].clickedInputPointer
				)
			);

			joyconsVec[joyconCelPressedIndex].clickedInputPointer = NULL;
			
		}
		else {
			clickedButtonOscMessage = "";
			clickedButtonPos.x = NULL;
			clickedButtonPos.y = NULL;
		}

		if(joyconsVec[joyconCelPressedIndex].clickedInputPointer == NULL && clickedButtonOscMessage == "")
			joyconCelPressedIndex = -1;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	winWidth = w;
	winHeight = h;

	updateJoyconsDrawings();
	guiControl.setPosition(w - guiWidth - BORDER, BORDER);
	guiConfigVirtualJoycons.setPosition(guiControl.getPosition().x - guiWidth - BORDER, BORDER);
	guiJoyconsList.setPosition(BORDER, BORDER);
	guiShortcuts.setPosition((winWidth / 2) - guiWidth, (winHeight / 2) - (guiShortcuts.getHeight() / 2));
	guiGraphHelp.setPosition(guiShortcuts.getPosition().x + guiWidth + BORDER, (winHeight / 2) - (guiGraphHelp.getHeight() / 2));
	guiGraphConfig.setPosition(winWidth - guiWidth - BORDER, winHeight - (5 * guiLineHeight) - BORDER);
}

void ofApp::exit() {
	disconnectAndDisposeAll();
}

void ofApp::checkAllButtonStates() {
	if (setUpdateConnected) {
		instantiateConnectedJoycons();
		setupGuiControl();
		setupGuiJoyconsList();
		cout << "Update ended. " << numDevicesConnected << " devices connected." << endl;
		if (numDevicesConnected > 0) {
			cout << "Device ids go from " << (numDevicesConnectedSum - numDevicesConnected) << " to ";
			cout << (numDevicesConnectedSum - numDevicesConnected) + (numDevicesConnected - 1) << "." << endl;
		}
		setUpdateConnected = false;
		framesWaited = 0;
		updateJoyconsDrawings();
	}

	if (framesWaited == framesToWait) {
		if (updateConnected || executeUpdateConnected) {
			cout << "Researching for connected joycons..." << endl;
			setGuiWithMessage(guiControl, "Reconnecting joycons...");
			setUpdateConnected = true;
			executeUpdateConnected = false;
			showGuiJoyconsList = false;
			guiJoyconsList.clear();
		}
		else if (disconnectAndDispose || executeDisconnectAndDispose) {
			framesWaited = 0;
			disconnectAndDisposeAll();
			setupGuiJoyconsList();
			executeDisconnectAndDispose = false;
		}
		else if (useVirtualJoycons != lastUseVirtualJoyconsValue) {
			framesWaited = 0;
			setupGuiJoyconsList();
			lastUseVirtualJoyconsValue = useVirtualJoycons;
			updateJoyconsDrawings();
		}
		else if (useEulerOrientation != lastUseEulerOrientationValue) {
			framesWaited = 0;
			for (int index = 0; index < joyconsVec.size(); index++) {
				joyconsVec[index].useEulerOrientation = useEulerOrientation;
				joyconsVec[index].clearNotUsedGraphValues();
			}
			lastUseEulerOrientationValue = useEulerOrientation;
		}
		else if (calibrateJoycons != lastCalibrateJoyconsValue) {
			framesWaited = 0;
			for (int index = 0; index < numDevicesConnected; index++) {
				joyconsVec[index].calibrateMotion(calibrateJoycons);
			}
			lastCalibrateJoyconsValue = calibrateJoycons;
		}
		else if (oscOnly != lastOscOnlyValue) {
			framesWaited = 0;
			lastOscOnlyValue = oscOnly;
		}
		else if (showGUIGraphConfig != lastShowGUIGraphConfigValue) {
			framesWaited = 0;
			lastShowGUIGraphConfigValue = showGUIGraphConfig;
		}
		else if (showShortcutsHelp != lastShowShortcutsHelpValue) {
			framesWaited = 0;
			lastShowShortcutsHelpValue = showShortcutsHelp;
		}
		else if (addVirtualJoycon || executeAddVirtualJoycon) {
			framesWaited = 0;
			Joycon newJoycon(VIRTUALJOY_DEVICEID, numVirtualJoycons, useEulerOrientation, guiColor.a, font);
			joyconsVec.push_back(newJoycon);
			numVirtualJoycons++;
			setupGuiJoyconsList();
			virtualJoyconsAddedLabel.operator=(ofToString(numVirtualJoycons));
			executeAddVirtualJoycon = false;
		}
		else if (removeAVirtualJoycon || executeRemoveAVirtualJoycon) {
			framesWaited = 0;
			if (numVirtualJoycons > 0) {
				for (int index = joyconsVec.size() - 1; index >= 0; index--) {
					if (joyconsVec[index].isVirtual) {
						numVirtualJoycons--;
						joyconsVec.erase(joyconsVec.begin() + index);
						break;
					}
				}
				setupGuiJoyconsList();
				virtualJoyconsAddedLabel.operator=(ofToString(numVirtualJoycons));
			}
			executeRemoveAVirtualJoycon = false;
			updateJoyconsDrawings();
		}
		else if (removeAllVirtual) {
			framesWaited = 0;
			if (numVirtualJoycons > 0) {
				for (int index = joyconsVec.size() - 1; index >= 0; index--) {
					if (joyconsVec[index].isVirtual) {
						numVirtualJoycons--;
						joyconsVec.erase(joyconsVec.begin() + index);
					}
				}
				setupGuiJoyconsList();
				virtualJoyconsAddedLabel.operator=(ofToString(numVirtualJoycons));
			}
			updateJoyconsDrawings();
		}
	}
	else if (framesWaited < framesToWait)
		framesWaited++;

	if (lastImuVectorSize != imuVectorSizeSlider) {
		for (int index = 0; index <= joyconsVec.size() - 1; index++) {
			joyconsVec[index].changeIMUVectorsSize(imuVectorSizeSlider);
		}

		lastImuVectorSize = imuVectorSizeSlider;
	}
}

void ofApp::instantiateConnectedJoycons() {
	vector<Joycon> virtualJoycons;
	disconnectAndDisposeAll();
	virtualJoycons = joyconsVec;
	joyconsVec.clear();

	numDevicesConnected = JslConnectDevices(); 
	connectedDevicesLabel.operator=(ofToString(numDevicesConnected));

	if (numDevicesConnected > 0) {
		joyconCelsWidth = winWidth / numDevicesConnected;
		joyconCelsHeight = winHeight / numDevicesConnected;

		int lastDeviceId = numDevicesConnected + numDevicesConnectedSum - 1;
		int devicePosition = 0; //the position of the device on 'joyconsVec'
		for (int deviceId = numDevicesConnectedSum; deviceId <= lastDeviceId; deviceId++, devicePosition++) {
			Joycon newJoycon(deviceId, devicePosition, useEulerOrientation, guiColor.a, font);
			joyconsVec.push_back(newJoycon);
		}

		numDevicesConnectedSum = numDevicesConnected + numDevicesConnectedSum;

		JslSetCallback(&getJslCallback); //_n2
	}

	if (virtualJoycons.size() > 0) { //_n3
		for (int index = 0; index < virtualJoycons.size(); index++) {
			joyconsVec.push_back(virtualJoycons[index]);
		}
	}
}

void ofApp::disconnectAndDisposeAll() {
	if (numDevicesConnected > 0) {
		JslDisconnectAndDisposeAll();
		for (int index = 0; index < joyconsVec.size(); index++) {
			if (!joyconsVec[index].isVirtual) {
				joyconsVec.erase(joyconsVec.begin() + index);
				index--;
			}
		}
		numDevicesConnected = 0;
		connectedDevicesLabel.operator=(ofToString(numDevicesConnected));
	}
}

void ofApp::updateJoyconsDrawings() {
	if (numSelectedJoycons > 0) {
		int joyconCelLines = 1;
		int joyconCelColumns = numSelectedJoycons;

		if (numSelectedJoycons > 2) {
			joyconCelColumns = (numSelectedJoycons / 2) + (numSelectedJoycons % 2);
			joyconCelLines = 2;
		}

		joyconCelsWidth = winWidth / joyconCelColumns;
		joyconCelsHeight = winHeight / joyconCelLines;

		int curColumn = 0;
		int curLine = 0;
		for (int index = 0; index < joyconsVec.size(); index++) {
			if (joyconsVec[index].GUIToggle && (!joyconsVec[index].isVirtual || (joyconsVec[index].isVirtual && useVirtualJoycons))) {
				if (curColumn == joyconCelColumns) {
					curColumn = 0;
					curLine++;
				}
				joyconsVec[index].updateDrawings(joyconCelsWidth, joyconCelsHeight, curColumn * joyconCelsWidth, curLine * joyconCelsHeight);
				curColumn++;
			}
		}
	}
	else {
		joyconCelsWidth = 0;
		joyconCelsHeight = 0;
	}
}

void ofApp::setupGuiControl() {
	guiControl.setup();
	guiControl.setName("GUIControl");
	guiControl.add(updateConnected.setup("updateConnected", guiWidth, guiLineHeight));
	guiControl.add(disconnectAndDispose.setup("disconnect&DisposeAll", guiWidth, guiLineHeight));
	guiControl.add(useVirtualJoycons.setup("useVirtualJoycons", false, guiWidth, guiLineHeight));
	guiControl.add(connectedDevicesLabel.setup("numConnectedDevices", connectedDevicesLabel, guiWidth, guiLineHeight));
	guiControl.add(calibrateJoycons.setup("calibrateJoycons", false, guiWidth, guiLineHeight));
	guiControl.add(oscOnly.setup("oscOnly", false, guiWidth, guiLineHeight));
	guiControl.add(showGUIGraphConfig.setup("graphConfig", false, guiWidth, guiLineHeight));
	guiControl.add(showShortcutsHelp.setup("shortcuts/help", false, guiWidth, guiLineHeight));
	connectedDevicesLabel.operator=(ofToString(numDevicesConnected));
	guiControl.setPosition(winWidth - guiWidth - BORDER, BORDER);

	guiControl.setBackgroundColor(guiColor);
	updateConnected.setBackgroundColor(guiColor);
	disconnectAndDispose.setBackgroundColor(guiColor);
	useVirtualJoycons.setBackgroundColor(guiColor);
	connectedDevicesLabel.setBackgroundColor(guiColor);
	calibrateJoycons.setBackgroundColor(guiColor);
	oscOnly.setBackgroundColor(guiColor);
	showGUIGraphConfig.setBackgroundColor(guiColor);
	showShortcutsHelp.setBackgroundColor(guiColor);
}

void ofApp::setupGuiJoyconsList(){
	guiJoyconsList.setup();
	guiJoyconsList.setName("JoyconsList");
	guiJoyconsList.setPosition(BORDER, BORDER);
	guiJoyconsList.setBackgroundColor(guiColor);

	if(joyconsVec.size() > 0){
		for(int index = 0; index < joyconsVec.size(); index++){
			if (!joyconsVec[index].isVirtual || (joyconsVec[index].isVirtual && useVirtualJoycons)) {
				string joyconGUILabel = joyconsVec[index].nameOnGUI;
				guiJoyconsList.add(joyconsVec[index].GUIToggle.setup(joyconGUILabel, joyconsVec[index].GUIToggle, guiWidth, guiLineHeight));
				joyconsVec[index].GUIToggle.setBackgroundColor(joyconsVec[index].joyconColor);
			}
		}
		showGuiJoyconsList = true;
	}
	else {
		showGuiJoyconsList = false;
	}

	numSelectedJoycons = numDevicesConnected;
	updateJoyconsDrawings();
}

void ofApp::setGuiWithMessage(ofxPanel& gui, string message) {
	gui.clear();
	gui.setBackgroundColor(guiColor);
	gui.add(messageLabel.setup(message, messageLabel, guiWidth, guiLineHeight));
}

void ofApp::openGeneralConfigWindow() {
	generalConfigWindowSettings.title = "General Config";
	generalConfigWindow = ofCreateWindow(generalConfigWindowSettings);
	ofAddListener(generalConfigWindow->events().draw, this, &ofApp::drawGeneralConfigWindow);
	ofAddListener(generalConfigWindow->events().mousePressed, this, &ofApp::mousePressedGeneralConfigWindow);
}

void ofApp::drawGeneralConfigWindow(ofEventArgs &args) {
	ofBackground(10);
	ofSetColor(50);
	ofRect(BORDER, BORDER, generalConfigWindowWidth - (2*BORDER), generalConfigWindowHeight - (2*BORDER));
}

void ofApp::mousePressedGeneralConfigWindow(ofMouseEventArgs &args) {
	string a = ofSystemTextBoxDialog("test", "resposetest");
}

void ofApp::openJoyconConfigWindow(Joycon &joyconToConfig) {
	joyconConfigWindowSettings.title = "Config Test: " + joyconToConfig.nameOnGUI;
	joyconConfigWindow = ofCreateWindow(joyconConfigWindowSettings);
	ofAddListener(joyconConfigWindow->events().draw, this, &ofApp::drawJoyconConfigWindow);
}

void ofApp::drawJoyconConfigWindow(ofEventArgs &args) {
}

/*
	n1- sets the local pointer 'joyOSCMapperPointer' with the adress of the new ofApp() created on main.cpp. 
		This is used with the 'getJslCallback' function, that receives the data from all joycons and updates them;
	n2- sets a callback function by wich JoyShockLibrary reports the data from all connected devices at their
		proper rate (for joycons, 66.67hz). This callback function needs to be reset every time after calling 
		'JslDisconnectAndDisposeAll()';
	n3- puts the virtual joycons always on the end of joyconsVec. The ideia here is to open an easy access from the 
		callback update function to the connected 'real' joycons, not needing any 'ifs' or 'fors' to find the 
		correct joycon to update;
*/