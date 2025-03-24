import React from 'react';
import Navbar from '../components/Navbar';
import VideoBackground from '../components/VideoBackground';
import OverlayLiane from '../components/OverlayLiane';

function CentralHome() {
  return (
    <div className="min-h-screen flex flex-col font-grotesk">
      <OverlayLiane />
      <Navbar />
      <VideoBackground />
    </div>
  );
}

export default CentralHome;