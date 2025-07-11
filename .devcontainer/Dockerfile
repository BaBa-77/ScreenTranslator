FROM mcr.microsoft.com/devcontainers/java:17

# Install essential packages
RUN apt-get update && \
    apt-get install -y wget unzip

# Install Android SDK Command-line Tools
ENV ANDROID_SDK_ROOT=/opt/android-sdk
RUN mkdir -p $ANDROID_SDK_ROOT/cmdline-tools && \
    wget -q https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip -O cmdline-tools.zip && \
    unzip cmdline-tools.zip -d $ANDROID_SDK_ROOT/cmdline-tools && \
    mv $ANDROID_SDK_ROOT/cmdline-tools/cmdline-tools $ANDROID_SDK_ROOT/cmdline-tools/latest && \
    rm cmdline-tools.zip

# Set environment variables
ENV PATH=$PATH:$ANDROID_SDK_ROOT/cmdline-tools/latest/bin:$ANDROID_SDK_ROOT/platform-tools

# Accept licenses and install necessary SDK components
RUN yes | sdkmanager --licenses && \
    sdkmanager "platform-tools" "platforms;android-33" "build-tools;33.0.0"

# Install Kotlin Language Server
RUN wget -q https://github.com/fwcd/kotlin-language-server/releases/download/1.4.7/kotlin-language-server-1.4.7.zip -O /tmp/kls.zip && \
    unzip /tmp/kls.zip -d /opt/kotlin-language-server && \
    ln -s /opt/kotlin-language-server/bin/kotlin-language-server /usr/local/bin/kotlin-language-server && \
    rm /tmp/kls.zip
