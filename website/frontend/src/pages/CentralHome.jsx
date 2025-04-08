import React from 'react';
import Navbar from '../components/Navbar';
import VideoBackground from '../components/VideoBackground';
import OverlayLiane from '../components/OverlayLiane';

// CentralHome.jsx
function CentralHome() {
  return (
    <div className="min-h-screen flex flex-col font-grotesk">
      <Navbar />
      <VideoBackground />
    </div>
  );
}


export default CentralHome;