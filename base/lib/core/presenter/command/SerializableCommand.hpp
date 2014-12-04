#pragma once
#include <core/presenter/command/Command.hpp>

namespace iscore
{
	/**
	 * @brief The SerializableCommand class
	 * 
	 * Adds serialization & deserialization capabilities to Command.
	 * 
	 */
	class SerializableCommand : public Command
	{
		public:
			using Command::Command;
			
			QByteArray serialize();
			void deserialize(QByteArray&);

		protected:
			virtual void serializeImpl(QDataStream&) = 0;
			virtual void deserializeImpl(QDataStream&) = 0;
	};
}
