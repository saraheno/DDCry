#ifndef CRY_Plugin_H
#define CRY_Plugin_H

// Framework include files
#include "DDG4/Geant4InputAction.h"
#include "DD4hep/ComponentProperties.h"

#include <fstream>


//forward declaratiosn
class CRYSetup;
class CRYGenerator;


/// Base class to read cry events
class CRYEventReader : public dd4hep::sim::Geant4EventReader  {
public:
  /// Initializing constructor
  CRYEventReader(const std::string& nam);
  /// Default destructor
  virtual ~CRYEventReader();

  /// Read an event and fill a vector of CryParticles.
  virtual EventReaderStatus readParticles(int /*event_number*/, 
                                          Vertices& vertices,
                                          std::vector<Particle*>& particles);


  virtual EventReaderStatus setParameters( std::map< std::string, std::string > & parameters ) override ;

protected:

  std::unique_ptr<CRYSetup> m_setup{};
  std::unique_ptr<CRYGenerator> m_gen{};
  std::string m_dataFilePath{};
  std::string m_configFilePath{};

  void initialise();
};

dd4hep::sim::Geant4EventReader::EventReaderStatus
CRYEventReader::setParameters( std::map< std::string, std::string > & parameters ) {

  _getParameterValue(parameters, "DataFilePath", m_dataFilePath, std::string());
  
  if( not m_dataFilePath.empty() )  {
    dd4hep::printout(dd4hep::INFO,"CRYEventReader","--- Will read data from %s ", m_dataFilePath.c_str());
    initialise();
    return EVENT_READER_OK;
  }
  dd4hep::printout(dd4hep::ERROR,"CRYEventReader","--- Will DataFilePath not set, abort!" );
  return EVENT_READER_IO_ERROR;
    
}

void CRYEventReader::initialise() {
  dd4hep::printout(dd4hep::INFO,"CRYEventReader","--- Will read setup from from %s ", m_configFilePath.c_str());
  
  // Read the setup file into setupString
  std::ifstream inputFile;
  inputFile.open(m_configFilePath, std::ios::in);
  char buffer[1000];

  std::string setupString("");
  while ( !inputFile.getline(buffer,1000).eof()) {
    setupString.append(buffer);
    setupString.append(" ");
  }

  // Parse the contents of the setup file
  m_setup = std::make_unique<CRYSetup>(setupString, m_dataFilePath);
  m_gen = std::make_unique<CRYGenerator>(m_setup.get());

}

#endif // CRY_Plugin_H
