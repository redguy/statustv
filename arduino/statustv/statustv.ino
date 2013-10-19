#include <IRLib.h>

#include <avr/pgmspace.h>


IRsend transmitter;
IRrecv receiver(6);
IRdecode decoder;

const int CMD_BUFFER_SIZE = 16;
char cmd[CMD_BUFFER_SIZE+1] = "";
int cmd_idx = 0;

struct IR_Command {
  prog_char * name;
  prog_uint32_t data;
  prog_int16_t length;
};

PROGMEM prog_char cmd_power[] = "power";
PROGMEM prog_char cmd_rewind[] = "rewind";
PROGMEM prog_char cmd_play[] = "play";
PROGMEM prog_char cmd_pause[] = "pause";
PROGMEM prog_char cmd_forward[] = "forward";
PROGMEM prog_char cmd_stop[] = "stop";
PROGMEM prog_char cmd_back[] = "back";
PROGMEM prog_char cmd_red[] = "red";
PROGMEM prog_char cmd_green[] = "green";
PROGMEM prog_char cmd_yellow[] = "yellow";
PROGMEM prog_char cmd_blue[] = "blue";
PROGMEM prog_char cmd_volplus[] = "vol+";
PROGMEM prog_char cmd_volminus[] = "vol-";
PROGMEM prog_char cmd_input[] = "input";
PROGMEM prog_char cmd_pplus[] = "p+";
PROGMEM prog_char cmd_pminus[] = "p-";
PROGMEM prog_char cmd_repeat[] = "repeat";

PROGMEM prog_char unknown[] = "?";

//PROGMEM prog_char cmd_[] = "";

PROGMEM struct IR_Command commands_NEC[] = {
  { cmd_power, 0x20DF10EF, 32 },
  { cmd_rewind, 0x20DFF10E, 32 },
  { cmd_play, 0x20DF0DF2, 32 },
  { cmd_pause, 0x20DF5DA2, 32 },
  { cmd_forward, 0x20DF718E, 32 },
  { cmd_stop, 0x20DF8D72, 32 },
  { cmd_back, 0x20DF14EB, 32 },
  { cmd_red, 0x20DF4EB1, 32 },
  { cmd_green, 0x20DF8E71, 32 },
  { cmd_yellow, 0x20DFC639, 32 },
  { cmd_blue, 0x20DF8679, 32 },
  { cmd_volplus, 0x20DF40BF, 32},
  { cmd_volminus, 0x20DFC03F, 32},
  { cmd_input, 0x20DFD02F, 32},
  { cmd_pplus, 0x20DF00FF, 32},
  { cmd_pminus, 0x20DF807F, 32},  
  { cmd_repeat, 0xFFFFFFFF, 32}, 
  { NULL, 0, 0}
};

struct IR_Command * ir_commands = commands_NEC;
IRTYPES ir_type = NEC;

struct IR_Command * command_by_name(char * name) {
  prog_char * s;
  for (struct IR_Command * cmd = ir_commands; (s = (prog_char *)pgm_read_word(&cmd->name)) != NULL ; cmd++) {
    if (!strcmp_P(name, s)) {
      return cmd;
    }
  }
  return NULL;
}

struct IR_Command * command_by_data(prog_uint32_t data) {
  for (struct IR_Command * cmd = ir_commands; (prog_char *)pgm_read_word(&cmd->name) != NULL ; cmd++) {
    if (data == pgm_read_dword(&cmd->data)) {
      return cmd;
    }
  }
  return NULL;
}  

prog_char * command_name_by_data(prog_uint32_t data) {
  struct IR_Command * cmd = command_by_data(data);
  if (cmd) {
    return (prog_char *)pgm_read_word(&cmd->name);
  }
  else {
    return NULL;
  }
}

void error(const char * msg) { 
  Serial.print("! ");
  Serial.println(msg);
}

void transmit(char * command) {
  struct IR_Command * cmd = command_by_name(command);
  if (cmd) {
    transmitter.send(ir_type, pgm_read_dword(&cmd->data), pgm_read_word(&cmd->length));
  }
  else {
    error("ir command not found");
    return;
  }
}

char * next_token(char * current) {
  while (*current && *current != ' ')
    *current++;
  while (*current == ' ')
    *current++;
   return current;
}

void process_serial_command() {
  char * param = next_token(cmd);
  
  switch (*cmd) {
  case 't': 
    transmit(param);
    break;
  }
  receiver.enableIRIn();();
}
  
int check_serial_command() {
  int c;
  while( ( c = Serial.read()) != -1) {
    if (c == '\r' || c == '\n') {
      cmd[cmd_idx] = 0;
      cmd_idx = 0;
      return 1;
    }

    cmd[cmd_idx++] = (char)c;
    if (cmd_idx == CMD_BUFFER_SIZE) {
      error("cmd buffer overflow");
      cmd_idx = 0;
    }
  }
  return 0;
}

void puts_P(prog_char * s) {
  for (prog_char c; (c = pgm_read_byte(s)); s++) Serial.write(c);
}

void panic(int on, int off = -1) {
  if (off < 0) {
    off = on;
  }
  pinMode(13, OUTPUT);
  while(true) {
    digitalWrite(13,HIGH);
    delay(100 * on);
    digitalWrite(13,LOW);
    delay(100 * off);
  }
}
  
void process_ir() {
  decoder.decode();//decode the signal
  Serial.print("R ");
  Serial.print(decoder.value,HEX);
  Serial.print(' ');
  prog_char * name = command_name_by_data(decoder.value);
  if (name) {
    puts_P(name);
  }
  else {
    puts_P(unknown);
  }
  Serial.println("");
  receiver.resume(); //restart the receiver
}

void setup() {
  Serial.begin(115200);//We will read the output on the serial monitor
  receiver.enableIRIn(); // Start the receiver
}  

void loop() {
  if (check_serial_command()) {
    process_serial_command();
  }
  if (receiver.GetResults(&decoder)) {
    process_ir();
  }
}

