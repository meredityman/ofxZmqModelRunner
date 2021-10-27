5
import argparse

class PortAction(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        if not 0 < values < 2**16:
            raise argparse.ArgumentError(self, "port numbers must be between 0 and 2**16")
        setattr(namespace, self.dest, values)


class AddressAction(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        if not 0 < values < 2**16:
            raise argparse.ArgumentError(self, "port numbers must be between 0 and 2**16")
        setattr(namespace, self.dest, values)



parser = argparse.ArgumentParser()
subparsers = parser.add_subparsers(dest="cmd")

tcp_parser = subparsers.add_parser('tcp')
parser.add_argument('--port',
                        help='Port number to connect to',
                        default=5732,
                        type=int,
                        action=PortAction,
                        metavar="{0..65535}")

parser.add_argument('--ip',
                        help='Address to connect to',
                        default="0.0.0.0")

ipc_parser= subparsers.add_parser('ipc')

parser.add_argument('--address',
                        help='Port number to connect to',
                        default="/tmp/ofxmodelrunner")


def parse_args():
    args = parser.parse_args()

    if args.cmd == "tcp":
        args.socket_addr = f"tcp://{args.ip}:{args.port}"
    elif args.cmd == "ipc":
        args.socket_addr = f"ipc://{args.address}"
    else:
        print(f"{args.cmd} not recognised")
        exit(-1)

    return args