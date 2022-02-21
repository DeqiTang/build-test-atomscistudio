/************************************************************************
 *
 * Atom Science Studio
 * Copyright (C) 2022  Deqi Tang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/



// Information of the atomic color:
// http://jmol.sourceforge.net/jscolors/
// Jmol is an excellent open source viewer for chemical
// structure, licensed under LGPL.


#ifndef MODELING_ATOMIC_COLOR_H_
#define MODELING_ATOMIC_COLOR_H_

#include <vector>
#include <map>
#include <string>


class AtomicColor {
public:
  AtomicColor() {
    this->_initialize();
  };
  ~AtomicColor() {};

  std::map<std::string, std::vector<int>> jmol;

private:
  void _initialize();

};

inline void AtomicColor::_initialize() {
this->jmol["H"] = std::vector<int>{255, 255, 255};
this->jmol["He"] = std::vector<int>{217, 255, 255};
this->jmol["Li"] = std::vector<int>{204, 128, 255};
this->jmol["Be"] = std::vector<int>{194, 255, 0};
this->jmol["B"] = std::vector<int>{255, 181, 181};
this->jmol["C"] = std::vector<int>{144, 144, 144};
this->jmol["N"] = std::vector<int>{48, 80, 248};
this->jmol["O"] = std::vector<int>{255, 13, 13};
this->jmol["F"] = std::vector<int>{144, 224, 80};
this->jmol["Ne"] = std::vector<int>{179, 227, 245};
this->jmol["Na"] = std::vector<int>{171, 92, 242};
this->jmol["Mg"] = std::vector<int>{138, 255, 0};
this->jmol["Al"] = std::vector<int>{191, 166, 166};
this->jmol["Si"] = std::vector<int>{240, 200, 160};
this->jmol["P"] = std::vector<int>{255, 128, 0};
this->jmol["S"] = std::vector<int>{255, 255, 48};
this->jmol["Cl"] = std::vector<int>{31, 240, 31};
this->jmol["Ar"] = std::vector<int>{128, 209, 227};
this->jmol["K"] = std::vector<int>{143, 64, 212};
this->jmol["Ca"] = std::vector<int>{61, 255, 0};
this->jmol["Sc"] = std::vector<int>{230, 230, 230};
this->jmol["Ti"] = std::vector<int>{191, 194, 199};
this->jmol["V"] = std::vector<int>{166, 166, 171};
this->jmol["Cr"] = std::vector<int>{138, 153, 199};
this->jmol["Mn"] = std::vector<int>{156, 122, 199};
this->jmol["Fe"] = std::vector<int>{224, 102, 51};
this->jmol["Co"] = std::vector<int>{240, 144, 160};
this->jmol["Ni"] = std::vector<int>{80, 208, 80};
this->jmol["Cu"] = std::vector<int>{200, 128, 51};
this->jmol["Zn"] = std::vector<int>{125, 128, 176};
this->jmol["Ga"] = std::vector<int>{194, 143, 143};
this->jmol["Ge"] = std::vector<int>{102, 143, 143};
this->jmol["As"] = std::vector<int>{189, 128, 227};
this->jmol["Se"] = std::vector<int>{255, 161, 0};
this->jmol["Br"] = std::vector<int>{166, 41, 41};
this->jmol["Kr"] = std::vector<int>{92, 184, 209};
this->jmol["Rb"] = std::vector<int>{112, 46, 176};
this->jmol["Sr"] = std::vector<int>{0, 255, 0};
this->jmol["Y"] = std::vector<int>{148, 255, 255};
this->jmol["Zr"] = std::vector<int>{148, 224, 224};
this->jmol["Nb"] = std::vector<int>{115, 194, 201};
this->jmol["Mo"] = std::vector<int>{84, 181, 181};
this->jmol["Tc"] = std::vector<int>{59, 158, 158};
this->jmol["Ru"] = std::vector<int>{36, 143, 143};
this->jmol["Rh"] = std::vector<int>{10, 125, 140};
this->jmol["Pd"] = std::vector<int>{0, 105, 133};
this->jmol["Ag"] = std::vector<int>{192, 192, 192};
this->jmol["Cd"] = std::vector<int>{255, 217, 143};
this->jmol["In"] = std::vector<int>{166, 117, 115};
this->jmol["Sn"] = std::vector<int>{102, 128, 128};
this->jmol["Sb"] = std::vector<int>{158, 99, 181};
this->jmol["Te"] = std::vector<int>{212, 122, 0};
this->jmol["I"] = std::vector<int>{148, 0, 148};
this->jmol["Xe"] = std::vector<int>{66, 158, 176};
this->jmol["Cs"] = std::vector<int>{87, 23, 143};
this->jmol["Ba"] = std::vector<int>{0, 201, 0};
this->jmol["La"] = std::vector<int>{112, 212, 255};
this->jmol["Ce"] = std::vector<int>{255, 255, 199};
this->jmol["Pr"] = std::vector<int>{217, 255, 199};
this->jmol["Nd"] = std::vector<int>{199, 255, 199};
this->jmol["Pm"] = std::vector<int>{163, 255, 199};
this->jmol["Sm"] = std::vector<int>{143, 255, 199};
this->jmol["Eu"] = std::vector<int>{97, 255, 199};
this->jmol["Gd"] = std::vector<int>{69, 255, 199};
this->jmol["Tb"] = std::vector<int>{48, 255, 199};
this->jmol["Dy"] = std::vector<int>{31, 255, 199};
this->jmol["Ho"] = std::vector<int>{0, 255, 156};
this->jmol["Er"] = std::vector<int>{0, 230, 117};
this->jmol["Tm"] = std::vector<int>{0, 212, 82};
this->jmol["Yb"] = std::vector<int>{0, 191, 56};
this->jmol["Lu"] = std::vector<int>{0, 171, 36};
this->jmol["Hf"] = std::vector<int>{77, 194, 255};
this->jmol["Ta"] = std::vector<int>{77, 166, 255};
this->jmol["W"] = std::vector<int>{33, 148, 214};
this->jmol["Re"] = std::vector<int>{38, 125, 171};
this->jmol["Os"] = std::vector<int>{38, 102, 150};
this->jmol["Ir"] = std::vector<int>{23, 84, 135};
this->jmol["Pt"] = std::vector<int>{208, 208, 224};
this->jmol["Au"] = std::vector<int>{255, 209, 35};
this->jmol["Hg"] = std::vector<int>{184, 184, 208};
this->jmol["Tl"] = std::vector<int>{166, 84, 77};
this->jmol["Pb"] = std::vector<int>{87, 89, 97};
this->jmol["Bi"] = std::vector<int>{158, 79, 181};
this->jmol["Po"] = std::vector<int>{171, 92, 0};
this->jmol["At"] = std::vector<int>{117, 79, 69};
this->jmol["Rn"] = std::vector<int>{66, 130, 150};
this->jmol["Fr"] = std::vector<int>{66, 0, 102};
this->jmol["Ra"] = std::vector<int>{0, 125, 0};
this->jmol["Ac"] = std::vector<int>{112, 171, 250};
this->jmol["Th"] = std::vector<int>{0, 186, 255};
this->jmol["Pa"] = std::vector<int>{0, 161, 255};
this->jmol["U"] = std::vector<int>{0, 143, 255};
this->jmol["Np"] = std::vector<int>{0, 128, 255};
this->jmol["Pu"] = std::vector<int>{0, 107, 255};
this->jmol["Am"] = std::vector<int>{84, 92, 242};
this->jmol["Cm"] = std::vector<int>{120, 92, 227};
this->jmol["Bk"] = std::vector<int>{138, 79, 227};
this->jmol["Cf"] = std::vector<int>{161, 54, 212};
this->jmol["Es"] = std::vector<int>{179, 31, 212};
this->jmol["Fm"] = std::vector<int>{179, 31, 186};
this->jmol["Md"] = std::vector<int>{179, 13, 166};
this->jmol["No"] = std::vector<int>{189, 13, 135};
this->jmol["Lr"] = std::vector<int>{199, 0, 102};
this->jmol["Rf"] = std::vector<int>{204, 0, 89};
this->jmol["Db"] = std::vector<int>{209, 0, 79};
this->jmol["Sg"] = std::vector<int>{217, 0, 69};
this->jmol["Bh"] = std::vector<int>{224, 0, 56};
this->jmol["Hs"] = std::vector<int>{230, 0, 46};
this->jmol["Mt"] = std::vector<int>{235, 0, 38};
}


#endif // MODELING_ATOMIC_COLOR_H_
