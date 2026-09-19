#include "protocol/JSONStatusWriter.h"

#include <Arduino.h>

namespace node
{

    JSONStatusWriter::JSONStatusWriter(
        ITransport& transport)
        : m_transport(transport)
    {
    }


    void JSONStatusWriter::begin()
    {
        // Início da mensagem:
        //
        // DATA:{"n":{
        //
        // "n" = nodes

        writeText("DATA:{\"n\":{");

        m_firstNode = true;
    }


    void JSONStatusWriter::beginNode(
        char nodeId)
    {
        // Se não for o primeiro Node,
        // precisamos separar com vírgula.
        //
        // Exemplo:
        //
        // "l":{...},"t":{...}

        if (!m_firstNode)
        {
            writeChar(',');
        }

        m_firstNode = false;

        // Abre o Node:
        //
        // "l":{

        writeChar('"');
        writeChar(nodeId);
        writeText("\":{");

        // Cada Node começa sem nenhuma
        // propriedade escrita.
        m_firstProperty = true;
    }


    void JSONStatusWriter::writeProperty( char propertyId, const Value& value)
    {
        // Se já existe uma propriedade,
        // separa a próxima com vírgula.
        //
        // Exemplo:
        //
        // "s":true,"v":10

        if (!m_firstProperty)
        {
            writeChar(',');
        }

        m_firstProperty = false;

        // Nome da propriedade:
        //
        // "s":

        writeChar('"');
        writeChar(propertyId);
        writeText("\":");

        // Valor:
        //
        // true
        // 42
        // 24.75

        writeValue(value);
    }


    void JSONStatusWriter::endNode()
    {
        // Fecha:
        //
        // "l":{"s":true}
        //               ^

        writeChar('}');
    }


    void JSONStatusWriter::end()
    {
        // Fecha:
        //
        // {
        //   "n": {
        //       ...
        //   }
        // }
        //
        // e termina a mensagem com '\n'.

        writeText("}}\n");
    }


    void JSONStatusWriter::writeText(
        const char* text)
    {
        if (text == nullptr)
        {
            return;
        }

        while (*text != '\0')
        {
            uint8_t byte =
                static_cast<uint8_t>(*text);

            m_transport.write(
                &byte,
                1
            );

            ++text;
        }
    }


    void JSONStatusWriter::writeChar(
        char value)
    {
        uint8_t byte =
            static_cast<uint8_t>(value);

        m_transport.write(
            &byte,
            1
        );
    }


    void JSONStatusWriter::writeValue(
        const Value& value)
    {
        // Buffer temporário pequeno.
        //
        // Só existe na stack enquanto
        // esta função está sendo executada.
        char buffer[16]{};

        switch (value.type)
        {
            case ValueType::BOOLEAN:
            {
                // JSON possui boolean nativo.
                //
                // Portanto usamos:
                //
                // true
                // false
                //
                // e não 1 / 0.

                writeText(
                    value.data.booleanValue
                        ? "true"
                        : "false"
                );

                break;
            }


            case ValueType::INTEGER:
            {
                ltoa(
                    value.data.integerValue,
                    buffer,
                    10
                );

                writeText(buffer);

                break;
            }


            case ValueType::FLOAT:
            {
                // 0  -> largura automática
                // 2  -> duas casas decimais

                dtostrf(
                    value.data.floatValue,
                    0,
                    2,
                    buffer
                );

                writeText(buffer);

                break;
            }


            case ValueType::NONE:
            default:
            {
                writeText("null");

                break;
            }
        }
    }

} // namespace node