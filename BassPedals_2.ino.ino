/** 
 * This is an example that demonstrates the use a Transposer to transpose the
 * note events sent out by NoteButton%s.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 2-9: momentary push buttons (to ground) to use as "piano keys"
 * - 10: momentary push button (to ground) to transpose one semitone up
 * - 11: momentary push button (to ground) to transpose one semitone down
 * 
 * The internal pull-up resistor for the buttons will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - By default, the 8 push buttons on pins 2-9 send MIDI Note events for notes
 *   C4 - C5.
 * - If you press the push button on pin 10, all notes are transposed one 
 *   semitone up.
 * - If you press the push button on pin 11, all notes are transposed one 
 *   semitone down.
 * - If you press the push buttons on pins 10 and 11 simultaneously, the 
 *   transposer resets.
 * 
 * Mapping
 * -------
 * 
 * Route the Arduino MIDI port into a synth to play the notes.
 * 
 * Written by PieterP, 2019-10-26  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Select the serial port to use.
auto &serial = Serial;
// Instantiate a Serial MIDI interface at the default MIDI baud rate.
SerialMIDI_Interface<decltype(serial)> midi = {serial, MIDI_BAUD};

// Instantiate a MIDI over USB interface.
//USBMIDI_Interface midi;

using namespace MIDI_Notes;

// Instantiate a Transposer that can transpose from one octave down to one
// octave up
Transposer<-1, +1> transposer(12);

// Instantiate a Selector to change the transposition
IncrementDecrementSelector<transposer.getNumberOfBanks()> selector = {
  transposer,
  {15, 16},
  Wrap::Clamp,
};
 
// Instantiate an array of NoteButton objects
Bankable::NoteButton buttons[] = {
  {transposer, 0, note(C, 3)}, 
  {transposer, 2, note(Db, 3)},
  {transposer, 3, note(D, 3)}, 
  {transposer, 4, note(Eb, 3)},
  {transposer, 5, note(E, 3)}, 
  {transposer, 6, note(F, 3)},
  {transposer, 7, note(Gb, 3)}, 
  {transposer, 8, note(G, 3)},
  {transposer, 9, note(Ab, 3)},
  {transposer, 10, note(A, 3)},
  {transposer, 11, note(Bb, 3)},
  {transposer, 12, note(B, 3)},
  {transposer, 14, note(C, 4)},
};

constexpr pin_t ledPin = 13;


void setup() {
  Control_Surface.begin();// Initialize Control Surface
  pinMode(ledPin, OUTPUT);
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
    auto checkButtonPressed = [](const Bankable::NoteButton &button) {
    return button.getButtonState() == Button::Pressed;
  };
  // If any of the push buttons is pressed
  bool pressed = std::any_of(std::begin(buttons), std::end(buttons),
                             checkButtonPressed);
  // Turn on the LED
  digitalWrite(ledPin, pressed);
}
