# Ani-Rem
A tool to passivly check for updates on seasons, for https://proxer.me

[![Travis Build Status](https://travis-ci.org/Skycoder42/AniRem.svg?branch=master)](https://travis-ci.org/Skycoder42/AniRem)
[![Appveyor Build status](https://ci.appveyor.com/api/projects/status/kqmf5af1ngg2phqo/branch/master?svg=true)](https://ci.appveyor.com/project/Skycoder42/seasonproxer/branch/master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/d75777e4fcda476a9dbc5bc0a40d2869)](https://www.codacy.com/app/Skycoder42/AniRem)

**Important:** If you have been using Ani-Rem (or Season-Proxer) pre Version 2.0.0, read the section [Migration](#Migration) below.

## Features
Proxer is awesome, but misses one feature: A notification when a new season/ova/etc is released for an anime you have already watched. This tools purpose is to do exactly that. You can

- Add Animes you have watched
	- Add them via their ID or URL
	- Add them by selection from your watch history
- Activate automatic season checks
	- The app notifies you when a new season/... gets released
- Secure End-to-End encrypted synchronization between multiple devices
- Available for all desktop platforms and Android (no iOS version due to API limitations & high appstore fees)

## Installation
There are multiple ways to get the application:

1. Arch Linux users: Have an AUR package: [`anirem`](https://aur.archlinux.org/packages/anirem/) or [`anirem-git`](https://aur.archlinux.org/packages/anirem-git/)
2. All platforms: Download the installer/apk from the [release page](https://github.com/Skycoder42/SeasonProxer/releases)
3. Android: While the apk is available as direct download, you can also get it via google play store: [Ani-Rem Google Play](https://play.google.com/store/apps/details?id=de.skycoder42.anirem)

## Migration
If you have been using Ani-Rem (or Season-Proxer) pre Version 2.0.0, the app will automatically migrate all you data to the new synchronisation backend. The conversion is lossless, but will corrupt old data, which means you wont be able to access your data with the older version anymore. Settings however are not migrated. If you changed any (like the update-interval) you need to do that again after the update.

Furthermore, if you had synchronized your data between multiple devices, you will have to reconnect all your devices, as the partners that your device is synchronized with cannot be migrated. THis can be done via the menu
option "Synchronization".

For all that still need the old version, it can be found as the "old" branch.

## Building it yourself
The application has a bunch of dependencies you need to install:
- Qt Modules:
	- [QtMvvm](https://github.com/Skycoder42/QtMvvm)
	- [QtDataSync](https://github.com/Skycoder42/QtDataSync)
	- [QtRestClient](https://github.com/Skycoder42/QtRestClient)
	- [QtJsonSerializer](https://github.com/Skycoder42/QtJsonSerializer)
	- [QtAutoUpdater](https://github.com/Skycoder42/QtAutoUpdater) (optional)
- Build Tools:
	- [qpmx](https://github.com/Skycoder42/qpmx)
	- [qpm](https://github.com/Cutehacks/qpm)
- A Proxer-API key (See https://proxer.me/wiki/Proxer_API)

Please not that some of those dependencies have dependencies as well. Once you satisfied them all, prepare the api key
by creating a file in the projects root directory named `proxer-api-key.h` with the following content:

```cpp
#define PROXER_API_KEY QByteArray("<your_api_key>")
```
