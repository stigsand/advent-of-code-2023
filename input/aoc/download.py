import aiofile
import aiohttp
import asyncio
import argparse
from pathlib import Path


async def read_cookie(file_path: Path) -> str:
    async with aiofile.async_open(file_path, "r") as afp:
        return (await afp.read()).strip()


async def download() -> None:
    parser = argparse.ArgumentParser(
        description="Script for downloading Advent of Code input files")
    parser.add_argument(
        "day", metavar='<day of month>', type=int, choices=range(1, 25 + 1))
    parser.add_argument(
        "-y", "--year", metavar='<year>', type=int, default=2023)
    group = parser.add_mutually_exclusive_group()
    group.add_argument("-s", "--session", metavar='<session>')
    group.add_argument(
        "-S", "--session-file", metavar='<file>', type=Path, default='.aocrc')
    args = parser.parse_args()

    year, day = args.year, args.day
    session_cookie = args.session \
        if args.session \
        else await read_cookie(args.session_file)

    async with aiohttp.ClientSession() as session:
        ua = "https://github.com/stigsand/advent-of-code-2023"
        async with \
                session.get(
                    f'https://adventofcode.com/{year}/day/{day}/input',
                    cookies={'session': session_cookie},
                    headers={'UserAgent': ua},
                    raise_for_status=True) as response, \
                aiofile.async_open(f"day-{day:02}", "wb+") as output_file:
            async for chunk in response.content.iter_chunked(512 * 1024):
                await output_file.write(chunk)


def main() -> str:
    asyncio.run(download())
