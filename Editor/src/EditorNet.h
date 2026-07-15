#pragma once

#include <string>
#include <vector>
#include <map>

// Thank you lord gaben for this wonderful library
#include <steam/steamnetworkingsockets.h>

#include <Core/Common.h>

namespace Refraction::Editor {
	class EditorNet {
	public:
		// Initialise in server mode
		static bool InitServer(uint16_t port);
		// Initialise in client mode
		static bool InitClient();
		// CLIENT: Connect to the server at the provided address
		static bool ConnectToServer(const SteamNetworkingIPAddr& serverAddress);

		// SERVER: Send a message to all clients
		static void BroadcastMessage(std::string message);

		// CLIENT: Get an incoming message
		// Also runs interface callbacks
		static std::vector<std::string> PollIncomingMessages();
		// CLIENT: Request a change to the server
		static void SendChangeRequest(std::string serialisedChange);

		// Disconnects and returns editor back to local mode
		static void Shutdown();
		static bool GetIsServer() { return IsServer; }
	private:
		// Server + client data
		///
		static ISteamNetworkingSockets* Interface;
		static bool IsServer;
		static bool ShouldClose;

		// Server data
		///
		static HSteamListenSocket ListenSocket;
		static HSteamNetPollGroup PollGroup;

		struct ClientProfile {
			std::string Name;
		};

		static std::map<HSteamNetConnection, ClientProfile> ClientMap;

		// Client data
		///
		static HSteamNetConnection Connection;


		static void ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* info);
	};
}
