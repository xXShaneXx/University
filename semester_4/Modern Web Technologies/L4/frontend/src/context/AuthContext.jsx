import React, { createContext, useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { loginUser, registerUser } from '../services/authService';

export const AuthContext = createContext();

export const AuthProvider = ({ children }) => {
  const [user, setUser] = useState(null);
  const [token, setToken] = useState(localStorage.getItem('token') || null);
  const [loading, setLoading] = useState(true);
  const navigate = useNavigate();

  useEffect(() => {
    const verifyToken = async () => {
      try {
        if (token) {
          const decoded = JSON.parse(atob(token.split('.')[1]));
          setUser(decoded);
        }
      } catch (err) {
        logout();
      } finally {
        setLoading(false);
      }
    };
    verifyToken();
  }, [token]);

  const login = async (credentials) => {
    try {
      const data = await loginUser(credentials);
      localStorage.setItem('token', data.token);
      setToken(data.token);
      return data;
    } catch (err) {
      throw err;
    }
  };

  const register = async (userData) => {
    try {
      await registerUser(userData);
    } catch (err) {
      throw err;
    }
  };

  const logout = () => {
    localStorage.removeItem('token');
    setToken(null);
    setUser(null);
    navigate('/login');
  };

  return (
    <AuthContext.Provider value={{ 
      user, 
      token, 
      login, 
      register, 
      logout, 
      loading 
    }}>
      {children}
    </AuthContext.Provider>
  );
};

export const useAuth = () => React.useContext(AuthContext);