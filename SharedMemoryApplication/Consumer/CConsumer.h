#pragma once

#include "..\Basics\Basics.h"

namespace Consumers {

	class CConsumer: public Applications::CApplication
	{
	public:
		CConsumer( );
		void processFrame( );

		CApplication& run( ) override;

	private:
		void createIpc( );
		bool toStop( ) const;
	};

}
