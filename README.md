# torus
Ultima online server emulator heavily influenced by SphereServer


#ParameterTypes:
    INTEGER
    STRING
    VECTOR<INTEGER>
    VECTOR<STRING>

Functions return a ParameterTypes or a VECTOR of ParameterTypes if there is more than one value to return.

#Operator behaviours:

##ADDITION (+)
    INTEGER:
        + STRING = The INTEGER gets transformed into a STRING and is prepended to the STRING, eg:  2 + "accounts" = "2accounts".
        + INTEGER = Both INTEGER are sum and the result is returned.
        + VECTOR<INTEGER> = Prepends the INTEGER into the VECTOR.
        + VECTOR<STRING> = Transform the INTEGER into a STRING and prepends it to the VECTOR.
    STRING:
        + STRING = Append the second STRING to the first one.
        + INTEGER = Transform the INTEGER into a STRING and Append it to the back of the STRING.
        + VECTOR<INTEGER> =
        + VECTOR<STRING> =
    VECTOR<INTEGER>:
        + STRING = Transform the VECTOR into a VECTOR<STRING> and Append the STRING.
        + INTEGER = Append the INTEGER at the end of the VECTOR.
        + VECTOR<INTEGER> = Append the second VECTOR at the end of the first one.
        + VECTOR<STRING> = Converts the first vector into a VECTOR<STRING> and Append at it's end the second one.
    VECTOR<STRING>:
        + STRING = Append the STRING at the end of the VECTOR.
        + INTEGER = Transform the INTEGER into a STRING and append it at the end of the VECTOR.
        + VECTOR<INTEGER> = Transform the second VECTOR into a VECTOR<STRING> and append it at the end of the first one.
        + VECTOR<STRING> = Append the second VECTOR at the end of the first one.
        

##SUBSTRACTION (-)
    INTEGER:
        - STRING = Removes the first N characters from the beggining of the STRING.
        - INTEGER = Normal INTEGER substraction.
        - VECTOR<INTEGER> = Removes the N first entries of the VECTOR from the beggining.
        - VECTOR<STRING> = Removes the N first entries of the VECTOR from the beggining.
    STRING:
        - STRING = Try to remove the characters of the second STRING from the first one.
        - INTEGER = Removes the last N characters from the end of the STRING.
        - VECTOR<INTEGER> = 
        - VECTOR<STRING> = Try to remove from the first VECTOR the STRINGS (characters?) from the second.
    VECTOR<INTEGER>:
        - STRING =
        - INTEGER = Removes the N first entries of the VECTOR from the last.
        - VECTOR<INTEGER> = Try to remove the entries of the second VECTOR from the first one.
        - VECTOR<STRING> =
    VECTOR<STRING>:
        - STRING = Try to remove the given STRING from the VECTOR.
        - INTEGER = Remove N entries from the end.
        - VECTOR<INTEGER> = (Venida arriba): Remove N characters from each entry of the FIRST vector matching with the same entry from the second.
            eg: VECTOR<STRING>{"hello", "world"} - VECTOR<INTEGER>{3,2} = VECTOR<STRING>{"he", "wor"} (odiame, oh si nena, odiame más).
        - VECTOR<STRING> = Try to remove the STRINGS (characters?) of the second VECTOR from the first one.
            eg: {"hello", "world"} - {"e", "l"} = {"ho", "word"}
                {"hello", "world"} - {"el", "world"} = {"hlo"}
##Multiplication (*)
    INTEGER:
        * STRING = N/A
        * INTEGER = Normal INTEGER multiplication.
    STRING:
        * STRING = N/A
        * INTEGER = Duplicate the STRING by N times, eg: 3 * "hello" = "hellohellohello"
    VECTOR<INTEGER>:
        * STRING = N/A
        * INTEGER = Multiply all the values by the INTEGER value, eg: {2,3,4} * 2 = {4,6,8}
        * VECTOR<INTEGER> = If both vectors have the same length, multiply each entry of the first VECTOR by the value of the same entry index from the second VECTOR.
        * VECTOR<STRING> = N/A
    VECTOR<STRING>:
        * STRING = N/A
        * INTEGER = Duplicate the STRING of each entry of the VECTOR by N times, eg: {"he","llo"] * 2 = {"hehe", "llollo"}
        * VECTOR<INTEGER> = IF the length of both VECTORS is the same, duplicate the STRING of each entry of the first VECTOR by N times the value of the same entry on the second VECTOR, eg: {"he","llo"] * {3, 2} = {"hehehe", "llollo"}
        * VECTOR<STRING> = N/A
##DIVISION (/)
    INTEGER:
        / STRING = N/A
        / INTEGER = Normal INTEGER division. 
    STRING:
        / STRING = Split the first STRING using the second one as separator, eg: "Hello World" / " " = vector{"Hello", "World"}
        / INTEGER = Splits the STRING in N strings with the size of the INTEGER, eg: "Hello World" / 3 = vector{"Hel","lo ", "wor", "ld"}
    VECTOR<INTEGER>:
        / STRING = N/A
        / INTEGER = Divide all the values by the INTEGER value, eg: {4,6,8} / 2 = {2,3,4}.
        / VECTOR<INTEGER> =
        / VECTOR<STRING> =
    VECTOR<STRING>:
        / STRING = N/A (Añadir esto supondría aplicar el operador / a cada entrada del vector, lo que nos daría vectores de vectores, que no están soportados).
        / INTEGER = N/A (Añadir esto supondría aplicar el operador / a cada entrada del vector, lo que nos daría vectores de vectores, que no están soportados).
        / VECTOR<INTEGER> = N/A (Añadir esto supondría aplicar el operador / a cada entrada del vector, lo que nos daría vectores de vectores, que no están soportados).
        / VECTOR<STRING> = N/A (Añadir esto supondría aplicar el operador / a cada entrada del vector, lo que nos daría vectores de vectores, que no están soportados).
##MODULO (%)
    INTEGER:
        % STRING = N/A
        % INTEGER = Normal INTEGER modulo.
    STRING:
        % STRING = Change the first "%s" found in the first STRING with the text of the second one.
        % INTEGER = Change the first "%d" found in the first STRING with the value of the INTEGER.
    VECTOR<INTEGER>:
        % STRING = N/A
        % INTEGER = Apply the modulo operation to all the entries of the VECTOR.
        % VECTOR<INTEGER> = If both vectors have the same length, apply the modulo to each entry of the first VECTOR by the value of the same entry index from the second VECTOR.
        % VECTOR<STRING> = N/A
    VECTOR<STRING>:
        % STRING = Apply the operator to each entry of the vector.
        % INTEGER = Apply the operator to each entry of the vector. 
        % VECTOR<INTEGER> = If both vectors have the same length, apply the modulo to each entry of the first VECTOR by the value of the same entry index from the second VECTOR.
        % VECTOR<STRING> = If both vectors have the same length, apply the modulo to each entry of the first VECTOR by the value of the same entry index from the second VECTOR.
##EXACT (===):
    Checks if both values have the same type, if so calls the EQUAL operator on them, with the following exception:
        STRING:
            === STRING = Check if both strings are equal with case sensitivity.
##EQUAL (==)
    INTEGER:
        == STRING = Create a copy of the INTEGER with a type of STRING and compare it to the STRING.
        == INTEGER = Check if both INTEGER have the same value.
    STRING:
        == STRING = Check if both STRING have the same text, returns TRUE if they match (Non case sensitive).
        == INTEGER = Check if the STRING has the length specified by INTEGER.
    VECTOR<INTEGER>:
        == STRING =
        == INTEGER = Check if both vectors have the same length and each entry of the second VECTOR matches with the same entry of the first one.
        == VECTOR<INTEGER> = If both vectors have the same length, check if the value of each entry from the first VECTOR is the same as the value of the same entry index from the second VECTOR.
        == VECTOR<STRING> = N/A
    VECTOR<STRING>:
        == STRING = Check if both vectors have the same length and each entry of the second VECTOR matches with the same entry of the first one.
        == INTEGER = Check if the length of each entry of the vector is equal to the value of the INTEGER.
        == VECTOR<INTEGER> = N/A
        == VECTOR<STRING> = If both vectors have the same length, check if the text of each entry from the first VECTOR is the same as the text of the same entry index from the second VECTOR (Non case sensitive).
##NOT EQUAL (!=):
    Does the same as the EQUAL operator but returns TRUE only if both values doesn't match.
##GREATER (>)
    INTEGER:
        > STRING = N/A
        > INTEGER = Check if the value of the first INTEGER is greater than the second's one.
    STRING:
        > STRING = Check if the length of the first STRING is greater than the second.
        > INTEGER = Check if the length of the STRING is greater than the value of INTEGER.
    VECTOR<INTEGER>:
        > STRING = N/A
        == INTEGER = Check if the length of each entry of the vector is greater than the value of the INTEGER.
    VECTOR<STRING>:
        > STRING = Check if both vectors have the same length and each entry of the second VECTOR is greater than the length of same entry of the first VECTOR.
        > INTEGER = Check if both vectors have the same length and each entry of the second VECTOR is greater than the length of same entry of the first VECTOR.
##GREATER OR EQUAL (>=)
    INTEGER:
        >= STRING = N/A
        >= INTEGER = Check if the value of the first INTEGER is greater or equal than the second's one.
    STRING:
        >= STRING = Check if the length of the first STRING is greater or equal than the second.
        >= INTEGER = Check if the length of the STRING is greater or equal than the value of INTEGER.
    VECTOR<INTEGER>:
        >= STRING = N/A
        == INTEGER = Check if the length of each entry of the vector is greater or equal than the value of the INTEGER.
    VECTOR<STRING>:
        >= STRING = Check if both vectors have the same length and each entry of the second VECTOR is greater or equal than the length of same entry of the first VECTOR.
        >= INTEGER = Check if both vectors have the same length and each entry of the second VECTOR is greater or equal than the length of same entry of the first VECTOR.
##LOWER (<)
    INTEGER:
        < STRING = N/A
        < INTEGER = Check if the value of the first INTEGER is lower than the second's one.
    STRING:
        < STRING = Check if the length of the first STRING is lower than the second.
        < INTEGER = Check if the length of the STRING is lower than the value of INTEGER.
    VECTOR<INTEGER>:
        < STRING = N/A
        == INTEGER = Check if the length of each entry of the vector is lower than the value of the INTEGER.
    VECTOR<STRING>:
        < STRING = Check if both vectors have the same length and each entry of the second VECTOR is lower than the length of same entry of the first VECTOR.
        < INTEGER = Check if both vectors have the same length and each entry of the second VECTOR is lower than the length of same entry of the first VECTOR.
##LOWER OR EQUAL (<=)
    INTEGER:
        <= STRING = N/A
        <= INTEGER = Check if the value of the first INTEGER is lower or equal than the second's one.
    STRING:
        <= STRING = Check if the length of the first STRING is lower or equal than the second.
        <= INTEGER = Check if the length of the STRING is lower or equal than the value of INTEGER.
    VECTOR<INTEGER>:
        <= STRING = N/A
        == INTEGER = Check if the length of each entry of the vector is lower or equal than the value of the INTEGER.
    VECTOR<STRING>:
        <= STRING = Check if both vectors have the same length and each entry of the second VECTOR is lower or equal than the length of same entry of the first VECTOR.
        <= INTEGER = Check if both vectors have the same length and each entry of the second VECTOR is lower or equal than the length of same entry of the first VECTOR.
##OR (|)
    INTEGER:
        - STRING = 
        - INTEGER = 
    STRING:
        - STRING = 
        - INTEGER =
    VECTOR<INTEGER>:
        + STRING =
        + INTEGER =
    VECTOR<STRING>:
        + STRING =
        + INTEGER =
##AND (&)
    INTEGER:
        - STRING = 
        - INTEGER = 
    STRING:
        - STRING = 
        - INTEGER =
    VECTOR<INTEGER>:
        + STRING =
        + INTEGER =
    VECTOR<STRING>:
        + STRING =
        + INTEGER =