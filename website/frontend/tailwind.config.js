/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    "./src/**/*.{js,jsx,ts,tsx}",
  ],
  theme: {
    extend: {
      fontFamily: {
        grotesk: ['Schibsted Grotesk', 'sans-serif'],
      },
      animation: {
        'bounce-slow': 'bounce 5s infinite ease-in-out',
      },
        
    },
  },
  plugins: [],
}