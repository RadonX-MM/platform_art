# /*
#  * Copyright 2015 The Android Open Source Project
#  *
#  * Licensed under the Apache License, Version 2.0 (the "License");
#  * you may not use this file except in compliance with the License.
#  * You may obtain a copy of the License at
#  *
#  *      http://www.apache.org/licenses/LICENSE-2.0
#  *
#  * Unless required by applicable law or agreed to in writing, software
#  * distributed under the License is distributed on an "AS IS" BASIS,
#  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  * See the License for the specific language governing permissions and
#  * limitations under the License.
#  */

.class public LE;
.super LA;
.implements LGreeter2;

# class E extends A implements Greeter2 {
#     public String SayHi() {
#         return "Hi2 ";
#     }
# }

.method public constructor <init>()V
    .registers 1
    invoke-direct {p0}, LA;-><init>()V
    return-void
.end method

.method public SayHi()Ljava/lang/String;
    .registers 1

    const-string v0, "Hi2 "
    return-object v0
.end method
