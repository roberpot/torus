/*
 * This file is part of Torus.
 * Torus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Torus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with Torus. If not, see <http://www.gnu.org/licenses/>.
 */

#include <library/system_headers.h>
#include <parser/compiler.h>
#include <network/networkmanager.h>
#include <core/torus.h>
#include <core/config.h>
#include <shell.h>
#include <debug_support/debug.h>
#include <core/slave.h>
#include <game/uo_files/uo_file_manager.h>
#include <game/server.h>

TorusShell torus_shell;
Torus torus;

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  torus_shell.start();
  TORUSSHELLECHO("Initializing Torus...");
  toruscfg.load_config_file("torus.ini");
  // torusacc.init();
  toruscompiler.add_file(std::string("scripts/torustables.tscp"));
  toruscompiler.compile();
  torusnet.start();
  if (torus.init()) {
    TORUSSHELLECHO("Initializing Torus... OK.");
    torus.mainloop();
  } else {
    TORUSSHELLECHO("Failed to initialize Torus, shuting down...")
  }
  TORUSSHELLECHO("Shutting down Torus...");
  torusnet.halt();
  torusnet.join();
  TORUSSHELLECHO("Shutting down Torus... OK.");
  torus_shell.join();
  return 0;
}