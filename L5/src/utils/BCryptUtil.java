package utils;

import org.mindrot.jbcrypt.BCrypt;

import java.io.Console;

public class BCryptUtil {
    public static String hashPassword(String password) {
        return BCrypt.hashpw(password, BCrypt.gensalt());
    }

    public static boolean checkPassword(String password, String hashed) {
        return BCrypt.checkpw(password, hashed);
    }

}
