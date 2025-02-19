#pragma once
#include <QByteArray>
#include <QDataStream>
#include <QBuffer>
#include <chrono>
#include <string>

#include <numeric>

#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/tools/ModelPathSerialization.hpp>
#include <core/application/ApplicationContext.hpp>

namespace iscore
{
    /**
     * @brief The Command class
     *
     * The base of the command system in i-score
     * It is timestamped, because we can then compare between clients.
     *
     * Maybe the NetworkPlugin should replace the Command by a TimestampedCommand instead ?
     * What if other plug-ins also want to add functionality ?
     *
     * Note: for mergeWith put two timestamps, one for the initial command (5 sec) and one for each
     * new command merged.
     */
    class Command
    {
        public:
            Command();
            virtual ~Command();

            virtual void undo() const = 0;
            virtual void redo() const = 0;

        protected:
            quint32 timestamp() const;
            void setTimestamp(quint32 stmp);

            iscore::ApplicationContext context;

        private:
            //TODO check if this is UTC
            std::chrono::milliseconds m_timestamp
            {  std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now().time_since_epoch()) };
    };
}
