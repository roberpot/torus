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

#ifndef __TORUS_GAME_UID_H_
#define __TORUS_GAME_UID_H_

#include <library/types.h>

#define UID_CLEAR       0x00000000
#define UID_CHAR        0x0
#define UID_ITEM        0x40000000
#define UID_RESOURCE    0x80000000
#define UID_UNUSED      0xFFFFFFFF

class Artifact;

class Uid {
private:
    udword_t _uid;    ///< numeric value.
protected:
    friend class Artifact;
    /**
    * @brief Sets the Uid for this obj.
    *
    * Creates a new Uid, also checks the current higher value to keep track of the last used value.
    * @param uid the Uid.
    */
    void set_uid(udword_t uid);
    static udword_t highest_uid;
    /**
    * @brief New Uid constructor.
    *
    * Creates a new Uid.
    */
    Uid();
    /**
    * @brief Find and set a new Uid for this object.
    *
    * Finds the first free Uid slot and sets it.
    */
    void find_new_uid();
    /**
    * @brief Remove this Uid.
    *
    * Remove this Uid from everywhere and adds it to free uids.
    */
    void free_uid();
    /**
    * @brief Sets the type of uid.
    *
    * @param mask The mask type (UID_ITEM, UID_RESOURCE...).
    */
    void set_uid_type(udword_t mask);
public:
    /**
    * @brief Existing Uid constructor.
    *
    * Receives a already existing Uid (loading from save?) and sets it.
    * @param uid the Uid.
    */
    Uid(udword_t uid);
    ~Uid();
    bool operator==(const Uid &uid) const;
    bool operator<(const Uid& uid) const;
    bool operator<=(const Uid& uid) const;
    bool operator>(const Uid& uid) const;
    bool operator>=(const Uid& uid) const;
    /**
    * @brief Returns this uid.
    *
    * @return this Uid.
    */
    udword_t get_uid() const;
    /**
    * @brief Returns the real uid.
    *
    * @return the real uid (without type masks).
    */
    udword_t get_uid_base() const;
    /**
    * @brief Checks if this Uid corresponds to a item.
    *
    * @return true if this Uid is from one item.
    */
    bool is_item() const;
    /**
    * @brief Checks if this Uid corresponds to a resource.
    *
    * @return true if this Uid is from one resource.
    */
    bool is_resource() const;
    /**
    * @brief Checks if this Uid corresponds to a char.
    *
    * @return true if this Uid is from one char.
    */
    bool is_char() const;
    /**
    * @brief Checks if this Uid is valid.
    *
    * @return true if this Uid is not invalid.
    */
    bool is_valid() const;
};

#endif // __TORUS_GAME_UID_H
