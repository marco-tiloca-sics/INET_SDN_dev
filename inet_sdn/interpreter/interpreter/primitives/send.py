"""
Send class models the 'send' primitive. It sends a packet to the next
layer in the communication stack.

The sintax of the primitive is:
send(packetName, delay)
 + packetName is the name of the packet to send (to the next layer in the communication stack);
 + delay is the forwarding delay.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


from utilities import error_arguments


class Send:
    """
    send(packetName, delay)
    """
    
    name = "Send"
    argv = []
    argc = 2
    
    def __init__(self, args):
        self.argv = args.split(":")

    def __str__(self):
        if self.argc != len(self.argv):
            error_arguments(self.name, self.argc, len(self.argv))
        
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Send</name>"
        xml += "\n\t\t\t\t<parameters>packetName:" + self.argv[0] + ":delay:" + self.argv[1] + "</parameters>"
        xml += "\n\t\t\t</action>"

        return xml
