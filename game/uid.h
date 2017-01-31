#ifndef _TORUS_GAME_UID_H_
#define _TORUS_GAME_UID_H_

#include "../core/types.h"

#define UID_CLEAR        0x00000000
#define UID_CHAR        0x0
#define UID_ITEM        0x40000000
#define UID_RESOURCE    0x80000000
#define UID_UNUSED        0xFFFFFFFF

class Uid {
private:
    t_udword _uid;    ///< numeric value.
protected:
    /**
    * @brief Sets the Uid for this obj.
    *
    * Creates a new Uid, also checks the current higher value to keep track of the last used value.
    * @param uid the Uid.
    */
    void set_uid(t_udword uid);
    static t_udword highest_uid;
    /**
    * @brief New Uid constructor.
    *
    * Creates a new Uid.
    */
    Uid();
    /**
    * @brief Existing Uid constructor.
    *
    * Receives a already existing Uid (loading from save?) and sets it.
    * @param uid the Uid.
    */
    Uid(t_udword uid);
    ~Uid();
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
    void set_uid_type(t_udword mask);
public:
    /**
    * @brief Returns this uid.
    *
    * @return this Uid.
    */
    t_udword get_uid();
    /**
    * @brief Returns the real uid.
    *
    * @return the real uid (without type masks).
    */
    t_udword get_uid_base();
    /**
    * @brief Checks if this Uid corresponds to a item.
    *
    * @return true if this Uid is from one item.
    */
    bool is_item();
    /**
    * @brief Checks if this Uid corresponds to a resource.
    *
    * @return true if this Uid is from one resource.
    */
    bool is_resource();
    /**
    * @brief Checks if this Uid corresponds to a char.
    *
    * @return true if this Uid is from one char.
    */
    bool is_char();
    /**
    * @brief Checks if this Uid is valid.
    *
    * @return true if this Uid is not invalid.
    */
    bool is_valid();
};

#endif //_TORUS_GAME_UID_H