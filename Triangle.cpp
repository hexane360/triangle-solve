#include "Triangle.h"
using namespace std;

Triangle::Triangle() {
    //cout << "Triangle::Triangle()\n";
    if (!_mainFont.loadFromFile(FONT_FILE)) {
        cout << "Failed to load font file \"" << FONT_FILE << "\"\n";
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 3; i++) { //pass same font object to all shapes
        _angles[i].setFont(_mainFont);
        _sides[i].setFont(_mainFont);
    }
    _messageBox.setFont(_mainFont);
    
    _angles[1].flipText(true); //flag for which way to align text
    _angles[2].flipText(false);
    
    reset();
}

/*
        ^  -  -  -
       /A\ B
      /   \
     b     c
    /       \
   /C       B\
   -----a-----
   note: clockwise is + rotation
*/

void debugSolveState(double sideArray[3], double angleArray[3]) {
    cout << "a: " << sideArray[0] << endl;
    cout << "b: " << sideArray[1] << endl;
    cout << "c: " << sideArray[2] << endl;
    cout << "A: " << angleArray[0] << endl;
    cout << "B: " << angleArray[1] << endl;
    cout << "C: " << angleArray[2] << endl;
}

void Triangle::resize(const Vector2f& size) {
	_windowWidth = size.x;
	_windowHeight = size.y;
	update();
}

void Triangle::update() {
    int angleCount = 0;
    int sideCount = 0;
    double angle[3] = {0,0,0};
    double side[3] = {0,0,0};
    cout << "\nBeginning solve:\n";
    if (_checkConstraints(side, angle, sideCount, angleCount)) return; //constrains as necessary, returns on error
    if (_lawOfCosines(side, angle, sideCount, angleCount)) return;
    
    double sineLaw = 0;
    double ambiguous = 0;
    int solveIteration = 0;
    while (angleCount < 3 || sideCount < 3) { //while there's still solving left
        if (solveIteration > 2) {
            message("Error: Solve loop did not finish.");
            return;
        }
        solveIteration++;
        if (angleCount == 2) { //use 180 = A + B + C
            cout << "Using interior angle sum\n";
            double sum = 0;
            int solveFor = 0;
            for (int i = 0; i < 3; i++) {
                if (!angle[i])
                    solveFor = i;
                sum += angle[i];
            }
            if (180 - sum <= 0.001) { //solved angle would be invalid
                message("The given values are impossible.");
                return;
            }
            angle[solveFor] = 180 - sum;
            cout << "angle[" << solveFor << "]: " << angle[solveFor] << endl;
            angleCount++;
        }
        if (!sineLaw) { //find angle/side pair to use with law of sines
            for (int i = 0; i < 3; i++) {
                if (side[i] && angle[i]) {
                    cout << "Got Law of Sines pair from " << i << endl;
                    sineLaw = side[i]/sin(angle[i]*TO_RADS);
                    break;
                }
            }
        }
        if (sineLaw) { // a/sin(A) = b/sin(B) = c/sin(C)
            for (int i = 0; i < 3; i++) {
                if (!side[i] && angle[i]) { //find side given opposite angle
                    side[i] = sin(angle[i]*TO_RADS)*sineLaw;
                    cout << "side[" << i << "]: " << side[i] << endl;
                    sideCount++;
                }
                if (side[i] && !angle[i]) { //find angle given opposite side
                    angle[i] = asin(side[i]/sineLaw)*TO_DEGS;
                    if (std::isnan(angle[i])) {
                        message("The given values are impossible.");
                        return;
                    }
	                double sum = 0;
	                for (int j = 0; j < 3; j++) {
	                    sum += angle[j];
	                }
	                
	                if (sum - 2*angle[i] < 0) { //ambiguous case
	                    if (!ambiguous) { //first time here, try to solve other places first
	                        ambiguous = angle[i];
	                        cout << "Skipping ambiguious case angle[" << i << "]: " << angle[i] << " or " << 180 - angle[i] << endl;
	                        angle[i] = 0;
	                    } else { //we've determined the triangle is ambiguous, just choose one possibility and go with it
	                        angleCount++;
	                        cout << "Confirmed ambiguious triangle. Choosing angle[" << i << "]: " << angle[i] << " vs " << 180 - angle[i] << endl;
	                    }
	                } else {
	                    angleCount++;
	                    cout << "angle[" << i << "]: " << angle[i] << endl;
	                }
                }
            }
        }
    }
    cout << "Solve finished\n";
    _drawSolved(side, angle);
}

bool Triangle::_checkConstraints(double side[3], double angle[3], int &sideCount, int &angleCount) {
	double maxSide = 0;
    for(int i=0; i<3; i++){
        angle[i] = _angles[i].isDriving() ? _angles[i].getValue() : 0;
        if (angle[i]) {
        	cout << "angle[" << i << "] given as " << angle[i] << endl;
            angleCount++;
        }
        side[i] = _sides[i].isDriving() ? _sides[i].getValue() : 0;
        if (side[i]) {
        	if (side[i] > maxSide) maxSide = side[i];
        	cout << "side[" << i << "] given as " << side[i] << endl;
            sideCount++;
        }
    }
    int constraints = angleCount + sideCount;
    cout << constraints << " constraints.\n";
    if (constraints < 3) { //underconstrained, just make up some constraints
        //if just AA, make an S
        if (!sideCount) {
            side[0] = _sides[0].getValue();
            cout << "Constraining side[0]: " << side[0] << endl;
            sideCount++; constraints++;
        }
        if (sideCount == 2 && angleCount == 0) { //if just SS, make an S to avoid invalid triangle
        	for (int i = 0; i < 3; i++) {
        		if (!side[i]) {
        			cout << "Constraining side[" << i << "]: " << maxSide << endl;
        			side[i] = maxSide;
        			sideCount++; constraints++;
        			return false;
        		}
        	}
        }
        double anglesLeft = 3 - angleCount; //unconstrained angles
        double angleSumLeft = 180;
        for (int i = 0; i < 3; i++) {
            angleSumLeft -= angle[i]; //finds remaining angles to make triangle
        }
        double idealAngle = angleSumLeft / anglesLeft;
        //guaranteed to make a valid (hopefully pretty) triangle
        //splits up remaining interior angle sum equally between unconstrained angles
        for (int i = 0; i < 3 && constraints < 3; i++) {
            if (!angle[i]) {
                angle[i] = idealAngle;
                cout << "Constraining angle[" << i << "]: " << angle[i] << endl;
                angleCount++; constraints++;
            }
        }
    }
    if (angleCount+sideCount > 3) { //overconstrained
        message("Too many set values");
        return true;
    }
    if (angleCount == 3) {
        message("Three angles are unsolvable.");
        return true;
    } else if (angleCount == 2 && angle[0] + angle[1] + angle[2] > 179.9999) {
        message("The entered angles are impossible.");
        return true;
    }
    return false;
}

bool Triangle::_lawOfCosines(double side[3], double angle[3], int &sideCount, int &angleCount) {
    //law of cosines if it applies
    //a^2 = b^2 + c^2 - 2*b*c*cos(A)
    if (sideCount == 2 && angleCount == 1) { //SSA or SAS
        if (!side[0] && angle[0]) { //SAS: find a using A, b and c
            cout << "Using SAS Law of Cosines\n";
            side[0] = sqrt(side[1]*side[1] + side[2]*side[2] - 2*side[1]*side[2]*cos(angle[0]*TO_RADS));
            cout << "side[0]: " << side[0] << endl;
            sideCount++;
        } else if (!side[1] && angle[1]) { // SAS: find b using a, B and c
            cout << "Using SAS Law of Cosines\n";
            side[1] = sqrt(side[0]*side[0] + side[2]*side[2] - 2*side[0]*side[2]*cos(angle[1]*TO_RADS));
            cout << "side[1]: " << side[1] << endl;
            sideCount++;
        } else if (!side[2] && angle[2]) { //SAS: find c using a, b and C
            cout << "Using SAS Law of Cosines\n";
            side[2] = sqrt(side[1]*side[1] + side[0]*side[0] - 2*side[1]*side[0]*cos(angle[2]*TO_RADS));
            cout << "side[2]: " << side[2] << endl;
            sideCount++;
        } else {
            cout << "SSA. Using Law of Sines\n";
        }
    }
    if (sideCount == 3) { //SSS
        cout << "Using SSS Law of Cosines\n";
        double maxSide = side[0];
        double perim = side[0];
        for (int i = 1; i < 3; i++) {
            perim += side[i];
            if (side[i] > maxSide)
                maxSide = side[i];
        }
        if (perim < 2*maxSide) { //c > a + b
            message("The entered values are impossible");
            return true;
        }
        for (int i = 0; i < 3; i++) {
        	if (!angle[i]) {
	        	angle[i] = TO_DEGS*acos((side[(i+1)%3]*side[(i+1)%3] + side[(i+2)%3]*side[(i+2)%3] - side[i]*side[i]) / (2*side[(i+1)%3]*side[(i+2)%3]));
    	    	angleCount++;
    	    	cout << "angle[" << i << "]: " << angle[i] << endl;
    	    }
        }
    }
    return false;
}

void Triangle::_drawSolved(double side[3], double angle[3]) {
    //find new positions
    //side a will be flat
    //triangle will be centered in y
    //triangle height is then b*sin(C)
    double triHeight = side[1]*sin(angle[2]*TO_RADS);
    double triLength = side[0]; //length of base leg
    bool leftCornerOffset = false;
    if (angle[1] > 90.0) {
        triLength += -side[2]*cos(angle[1]*TO_RADS); //side c sticks past base, offset to compensate
    } else if (angle[2] > 90.0) {
        triLength += -side[1]*cos(angle[2]*TO_RADS); //side b sticks past base
        leftCornerOffset = true; //left corner offset by b, use right instead
    }
    double scale = 0.8 / max( //fill 80% of screen
        triLength / static_cast<double>(_windowWidth), //base of triangle
        triHeight / static_cast<double>(_windowHeight) //height of triangle
    );
    for (int i = 0; i < 3; i++) {
        _sides[i].setScale(scale); //scale lines as well as positions
        _sides[i].setValue(side[i]);
        _angles[i].setValue(angle[i]);
        side[i] *= scale; //scale all our solved values
    }
    triHeight *= scale; //update length and height with scale
    triLength *= scale;
    
    _sides[0].flipText(false);
    _sides[1].flipText(angle[2] > 90.0); //if sides are past vertical, draw text other way
    _sides[2].flipText(angle[1] > 90.0);
    
    double leftCornerX = -triLength/2.0; //center bottom in x
    if (leftCornerOffset) { //left corner inset in
        leftCornerX = triLength/2.0 - side[0]; //right corner - length = left corner
    }
    double bottomY = triHeight/2.0; //and in y
    Vector2f posC = Vector2f(leftCornerX, bottomY); //bottom left corner
    //draw lines attached to posC
    _sides[0].setDrawPos(posC, 0.0);
    _sides[1].setDrawPos(posC, -angle[2]);
    
    //posA = posC + components of b = posC + <b*cos(C), -height>
    Vector2f posA = posC + Vector2f(side[1]*cos(angle[2]*TO_RADS), -triHeight);
    _sides[2].setDrawPos(posA, angle[1]);
    
    double bisectAngles[3];
    bisectAngles[0] = 180 - angle[2] - angle[0]/2.0; //rotate to lie along b, then to straddle A
    bisectAngles[1] = 180 + angle[1]/2.0; //halfway between both sides of B
    bisectAngles[2] = -angle[2]/2.0; //halfway between both sides of C
    
    _angles[0].flipText(bisectAngles[0] > 90); //draw text right side up
    
    _angles[0].setDrawPos(posA, bisectAngles[0]);
    _angles[1].setDrawPos(posC + Vector2f(side[0],0.0), bisectAngles[1]); //pos B
    _angles[2].setDrawPos(posC, bisectAngles[2]);
}

void Triangle::sendChar(char c) {
    if (_messageBox.displayed) { //cancel messagebox if open
        _messageBox.displayed = false;
        return;
    }
    
    for (int i = 0; i < 3; i++) {
        if (_sides[i].selected()) {
            _sides[i].sendChar(c);
            break;
        }
        if (_angles[i].selected()) {
            _angles[i].sendChar(c);
            break;
        }
    }
    if (c == '\n') //values have changed, recalculate
        update();
}

void Triangle::clickAt(const Vector2f &pos) { //select one dimension and deselect all others
    if (_messageBox.displayed) {
        if (_messageBox.contains(pos)) //clicked inside box, exit screen
            _messageBox.displayed = false;
        return;
    }
    bool selectedOne = false;
    for (int i = 0; i < 3; i++) {
        if (!selectedOne && _sides[i].contains(pos)) {
            _sides[i].select();
            selectedOne = true;
        } else
            _sides[i].cancelSelection(); //deselect if selected
        if (!selectedOne && _angles[i].contains(pos)) {
            _angles[i].select();
            selectedOne = true;
        } else
            _angles[i].cancelSelection(); //deselect
    }
}

void Triangle::draw(RenderTarget &target, RenderStates states) const { //fast draw loop
    //cout << "Triangle::draw()\n";
    for (int i = 0; i < 3; i++) {
        target.draw(_angles[i], states);
        target.draw(_sides[i], states);
    }
    target.draw(_messageBox, states);
}

void Triangle::clear() { //clears constraints
    if (_messageBox.displayed)
        _messageBox.displayed = false;
    for (int i = 0; i < 3; i++) {
        _sides[i].makeDriven();
        _sides[i].cancelSelection();
        _angles[i].makeDriven();
        _angles[i].cancelSelection();
    }
    update();
}

void Triangle::reset() { //resets all values to default
    if (_messageBox.displayed)
        _messageBox.displayed = false;
    for (int i = 0; i < 3; i++) {
        _sides[i].makeDriven();
        _sides[i].setValue(400);
        _angles[i].makeDriven();
        _angles[i].setValue(60);
    }
    update();
}

void Triangle::cancelSelection() { //resets current selection to driven
    if (_messageBox.displayed)
        _messageBox.displayed = false;
    for (int i = 0; i < 3; i++) {
        if (_sides[i].selected()) {
            _sides[i].cancelSelection();
            _sides[i].makeDriven();
        }
        if (_angles[i].selected()) {
            _angles[i].cancelSelection();
            _angles[i].makeDriven();
        }
    }
    update();
}

void Triangle::message(const sf::String &s) {
    _messageBox.display(s);
}
