import axios from 'axios';

const API_URL = '/api/auth';

export const loginUser = async (credentials) => {
    try {
        const response = await axios.post(`${API_URL}/login`, credentials);
        console.log(response.data)
        return response.data;
    } catch (error) {
        throw error.response.data;
    }
};

export const registerUser = async (userData) => {
    try {
        const response = await axios.post(`${API_URL}/register-user`, userData);
        return response.data;
    } catch (error) {
        throw error.response.data;
    }
};