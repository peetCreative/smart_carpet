void updateToActivateRegisters() {
  
  int activate_new = 0;
  for (int i = 0; i < NUM_STATES; i++) {

    Serial.println("Register " + String(register_to_activate[i]) + "\tRegister Status " + String(register_states[i]));
  
    if (register_states[i] == 1) {
      activate_new = activate_new | register_to_activate[i];
    }
  }
  activate = activate_new;

  Serial.println("ACTIVATE " + String(activate));
}
